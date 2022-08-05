#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionClose_file, &QAction::triggered, this, &MainWindow::closeFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    connect(ui->btn_save, &QPushButton::released, this, &MainWindow::saveFile);
    connect(ui->btn_open, &QPushButton::released, this, &MainWindow::openFile);
    connect(ui->btn_closef, &QPushButton::released, this, &MainWindow::closeFile);

    connect(ui->line_filename, &QLineEdit::textChanged, this, &MainWindow::filenameExists);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::saveFile()
{
    QString base_dir;
    QString save_dir;
    QString content_text = ui->text_edit_box->toPlainText();

    if (ui->line_filename->text() == "")
        base_dir = "/home";
    else
        base_dir = ui->line_filename->text();

    if (!this->fileExist(ui->line_filename->text().toStdString().c_str()))
    {
        save_dir = QFileDialog::getExistingDirectory(this, tr("Select a directory"), base_dir);
        if (save_dir.isNull() && save_dir.isEmpty())
            return;

        save_dir.append("/").append(ui->line_filename->text());
    }
    else
    {
        save_dir = ui->line_filename->text();
        base_dir = save_dir;
    }

    try
    {
        std::filesystem::path pathfile(save_dir.toStdString());
        std::ofstream file(pathfile);

        if (file.is_open())
        {
            file << content_text.toStdString();
            file.close();
        }
    }
    catch (const std::exception& e)
    {
        return;
    }

    QString message;
    message.append("File saved as: ").append(save_dir);

    ui->lbl_info->setText(message);

    ui->btn_closef->setDisabled(false);
    ui->line_filename->setText(save_dir);
    ui->line_filename->setDisabled(true);
}

void MainWindow::openFile()
{
    QString file_dir = QFileDialog::getOpenFileName(this, tr("Select a file"), "/home");
    ui->line_filename->setText(file_dir);

    QString file_text;

    if (file_dir == "")
    {
        ui->line_filename->setDisabled(false);
        ui->text_edit_box->setPlainText("");
        ui->lbl_info->setText("");
        return;
    }

    try
    {
        std::string line;
        std::stringstream buffer;
        std::filesystem::path path(file_dir.toStdString());
        std::ifstream file(path);
        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                buffer << line << std::endl;
            }

            file.close();
        }

        file_text.append(buffer.str().c_str());

        ui->text_edit_box->setPlainText(file_text);
    }
    catch (const std::exception& e)
    {
        return;
    }

    QString message;
    message.append("File opened: ").append(file_dir);

    ui->lbl_info->setText(message);

    ui->btn_closef->setDisabled(false);
    ui->line_filename->setDisabled(true);
}

void MainWindow::filenameExists()
{
    if (ui->line_filename->text() == "")
        ui->btn_save->setDisabled(true);
    else
        ui->btn_save->setDisabled(false);
}

void MainWindow::closeFile()
{
    ui->btn_save->setDisabled(true);
    ui->text_edit_box->setPlainText("");
    ui->btn_closef->setDisabled(true);
    ui->line_filename->setDisabled(false);
    ui->line_filename->setText("");

    ui->lbl_info->setText("File clossed.");
}

bool MainWindow::fileExist(std::string path)
{
    std::ifstream file(path);

    if (file.is_open())
        return true;

    return false;
}
