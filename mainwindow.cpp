#include <QtWidgets>
#include <QListWidgetItem>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Reloads items every time application is run
    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)  + "\\toDoFile.txt");

    if (!file.open(QIODevice::ReadWrite)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);

    while (!in.atEnd()) {
        QListWidgetItem* item = new QListWidgetItem(in.readLine(), ui->listWidget);
        ui->listWidget->addItem(item);

        // Read the checked/unchecked state from the file and set it for the item
        QString checkState = in.readLine();
        if (checkState == "Checked") {
            item->setCheckState(Qt::Checked);
            QFont font = item->font();
            font.setStrikeOut(true);
            item->setFont(font);
        } else {
            item->setCheckState(Qt::Unchecked);
            QFont font = item->font();
            font.setStrikeOut(false);
            item->setFont(font);
        }

        // Enable editing and checkability for the item
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
    }

    file.close();

    // Create a new QShortcut object with Enter as the shortcut key
    QShortcut *shortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);

    // Connect the activated() signal of the shortcut to the slot that handles the "Add" button click
    connect(shortcut, &QShortcut::activated, this, &MainWindow::on_btnAdd_clicked);

    // Set the focus policy of the "txtTask" widget to accept focus by pressing Enter
    ui->txtTask->setFocusPolicy(Qt::StrongFocus);
    ui->txtTask->setPlaceholderText("Type a task and press Enter");

    // Connect the itemClicked() signal of the listWidget to the slot that handles checking/unchecking items
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::on_listWidget_itemClicked);
}

MainWindow::~MainWindow()
{
    delete ui;

    QFile file(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)  + "\\toDoFile.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream out(&file);

    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        out << item->text() << "\n";

        if (item->checkState() == Qt::Checked) {
            out << "Checked\n";
        } else {
            out << "Unchecked\n";
        }
    }

    file.close();
}


void MainWindow::on_btnAdd_clicked()
{
    QListWidgetItem* item = new QListWidgetItem(ui->txtTask->text(), ui->listWidget);
    ui->listWidget->addItem(item);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->txtTask->clear();
    ui->txtTask->setFocus();
}

void MainWindow::on_btnRemove_clicked()
{
    for (int i = ui->listWidget->count() - 1; i >= 0; --i) {

        QListWidgetItem* item = ui->listWidget->item(i);

        if (item->checkState() == Qt::Checked) {
            delete item;
        }

    }
}

void MainWindow::on_btnSelectAll_clicked()
{
    for (int i = ui->listWidget->count() - 1; i >= 0; --i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState(Qt::Checked);
        QFont font = item->font();
        font.setStrikeOut(true);
        item->setFont(font);
    }
}

void MainWindow::on_btnUnselectAll_clicked()
{
    for (int i = ui->listWidget->count() - 1; i >= 0; --i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState(Qt::Unchecked);
        QFont font = item->font();
        font.setStrikeOut(false);
        item->setFont(font);
    }
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem* item)
{
    Qt::CheckState checkState = item->checkState();

    if (checkState == Qt::Checked) {
        QFont font = item->font();
        font.setStrikeOut(true);
        item->setFont(font);
    } else {
        QFont font = item->font();
        font.setStrikeOut(false);
        item->setFont(font);
    }
}
