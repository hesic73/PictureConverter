#ifndef IMAGEINFOLIST_H
#define IMAGEINFOLIST_H


#include <QWidget>
#include<QLabel>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QImageReader>
#include <QFileInfo>
#include<QListWidget>
#include<QListWidgetItem>
#include<QLayout>
#include<QSet>
#include<QPushButton>
#include<QFileDialog>
#include <QDir>
#include<QComboBox>
#include<QMessageBox>

#include<QDebug>

class ImageInfoList : public QWidget
{
    Q_OBJECT
public:
    explicit ImageInfoList(QWidget *parent = nullptr):QWidget{parent},outputDir(QDir::currentPath()){
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        QHBoxLayout *outputLayout = new QHBoxLayout();

        QPushButton *chooseFilesButton = new QPushButton("Choose Files", this);
        connect(chooseFilesButton, &QPushButton::clicked, this, &ImageInfoList::chooseFiles);

        QPushButton *clearButton = new QPushButton("Clear", this);
        connect(clearButton, &QPushButton::clicked, this, &ImageInfoList::clearList);

        QPushButton *chooseOutputDirButton = new QPushButton("Choose Output Directory", this);
        connect(chooseOutputDirButton, &QPushButton::clicked, this, &ImageInfoList::chooseOutputDirectory);

        QPushButton *convertButton = new QPushButton("Convert", this);
        connect(convertButton, &QPushButton::clicked, this, &ImageInfoList::convertImages);



        outputDirLabel = new QLabel("Output Directory: " + outputDir, this); // QLabel for displaying output directory
        outputFormatComboBox = new QComboBox(this); // QComboBox for selecting output format
        outputFormatComboBox->addItems(QStringList() << "PNG" << "JPEG" << "BMP" << "GIF"); // Add image format options


        outputLayout->addWidget(outputDirLabel);
        outputLayout->addWidget(outputFormatComboBox);

        buttonLayout->addWidget(chooseFilesButton);
        buttonLayout->addWidget(clearButton);
        buttonLayout->addWidget(chooseOutputDirButton);


        listWidget = new QListWidget(this);

        mainLayout->addLayout(outputLayout);
        mainLayout->addLayout(buttonLayout);
        mainLayout->addWidget(listWidget);
        mainLayout->addWidget(convertButton);

        // Enable drag & drop
        setAcceptDrops(true);
    }

protected:
    // Reimplement dragEnterEvent
    void dragEnterEvent(QDragEnterEvent *event) override {
        if (event->mimeData()->hasUrls()) {
            QList<QUrl> urls = event->mimeData()->urls();
            for (const QUrl &url : urls) {
                QString filePath = url.toLocalFile();
                if (QImageReader::imageFormat(filePath).isEmpty()) {
                    // Not an image file, ignore it
                    event->ignore();
                    return;
                }
            }
            event->acceptProposedAction();
        } else {
            event->ignore();
        }
    }

    void dropEvent(QDropEvent *event) override {
        const QMimeData *mimeData = event->mimeData();

        if (mimeData->hasUrls()) {
            QList<QUrl> urls = mimeData->urls();
            processUrls(urls);
            event->acceptProposedAction();
        }
    }


signals:


private:
    QListWidget* listWidget;
    QSet<QString> filePaths;
    QString outputDir;
    QLabel *outputDirLabel; // To display the selected output directory
    QComboBox *outputFormatComboBox; // To select the output image format

    void processUrls(const QList<QUrl> &urls) {
        for (const QUrl &url : urls) {
            QString filePath = url.toLocalFile();
            if (!filePaths.contains(filePath)) {
                QImageReader reader(filePath);
                if (!reader.format().isEmpty()) {
                    // It's an image file; add its info to the list
                    QImage image(filePath);
                    QListWidgetItem *item = new QListWidgetItem(listWidget);
                    item->setText(QString("%1\t%2x%3\t%4")
                                      .arg(filePath)
                                      .arg(image.width())
                                      .arg(image.height())
                                      .arg(QString(reader.format()).toUpper()));
                    listWidget->addItem(item);

                    // Add filePath to the set to ensure uniqueness
                    filePaths.insert(filePath);
                }
            }
        }
    }

    void chooseFiles() {
        QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select Image Files"), QString(),
                                                              tr("Images (*.png *.xpm *.jpg *.jpeg *.bmp *.gif)"));

        QList<QUrl> urls;
        for (const QString &fileName : fileNames) {
            urls.append(QUrl::fromLocalFile(fileName));
        }
        processUrls(urls);
    }

    void clearList() {
        listWidget->clear();
        filePaths.clear();
    }

    void chooseOutputDirectory() {
        QString dir = QFileDialog::getExistingDirectory(this, tr("Select Output Directory"),
                                                        outputDir,
                                                        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if (!dir.isEmpty()) {
            outputDir = dir;
            outputDirLabel->setText("Output Directory: " + outputDir);
        }
    }

    void convertImages() {
        QString selectedFormat = outputFormatComboBox->currentText().toLower(); // Get the selected output format

        for (const QString &filePath : qAsConst(filePaths)) {
            QImage image(filePath);
            if (!image.isNull()) { // Check if the image is valid
                QFileInfo fileInfo(filePath);
                QString baseName = fileInfo.baseName(); // Get the base name without the path and extension
                QString newFileName = outputDir + "/" + baseName + "." + selectedFormat; // Construct the new file name with the selected format
                image.save(newFileName, selectedFormat.toUpper().toUtf8()); // Save the image in the new format
            }
        }
        clearList();
        QMessageBox::information(this, "Conversion Complete", "All images have been converted.");
    }


};

#endif // IMAGEINFOLIST_H
