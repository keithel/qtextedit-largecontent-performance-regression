#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QPushButton;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {};

protected:
    void fillTextEdit(int numLines);

private:
    QPushButton* m_fillButton = nullptr;
    QTextEdit* m_textEdit = nullptr;
};
#endif // MAINWINDOW_H
