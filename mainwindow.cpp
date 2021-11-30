#include "mainwindow.h"
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>
#include <QtCore/QtMath>
#include <QtCore/QDebug>
#include <QtCore/QElapsedTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800, 600);

    m_textEdit = new QTextEdit(this);
    setCentralWidget(m_textEdit);

    QToolBar* toolbar = this->addToolBar("toolbar");
    QAction* fillAction = toolbar->addAction("Fill");
    connect(fillAction, &QAction::triggered, this, [this](){
        this->fillTextEdit(4000);
    });
    QAction* clearAction = toolbar->addAction("Clear");
    connect(clearAction, &QAction::triggered, m_textEdit, &QTextEdit::clear);

    connect(m_textEdit->document(), &QTextDocument::contentsChanged, this, [this](){
        int lineCount = m_textEdit->document()->lineCount();
        this->statusBar()->showMessage(QString::number(lineCount));
    });
}

void MainWindow::fillTextEdit(int numLines)
{
    QElapsedTimer et;
    et.start();
    QString loremIpsum(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore\n"
        "et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut\n"
        "aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse\n"
        "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa\n"
        "qui officia deserunt mollit anim id est laborum.\n");

    int nLipsumLines = loremIpsum.count("\n");
    int nLipsumCopies = qCeil(numLines / nLipsumLines);

    QTextCursor cursor = m_textEdit->textCursor();
    cursor.beginEditBlock();
    for (int i = 0; i < nLipsumCopies; i++)
    {
        cursor.insertText(loremIpsum);
    }

    // After text insertion, the cursor ends up positioned at the end of the inserted text.
    // If the block that is inserted is sufficiently long (in this example, 4000 lines),
    // cursor.endEditBlock() will take many seconds to complete (12.7 secs on my machine), hanging
    // the UI.
    // Setting the cursor position of the QTextEdit to the top of the document prior to calling
    // QTextCursor::endEditBlock() avoids the lengthy app hang, taking only ~700ms on my machine.
    //cursor.setPosition(0);
    //m_textEdit->setTextCursor(cursor);

    // Further investigation shows that the hang only occurs when the number of lines in the
    // document is less than the number of lines that can be displayed in the QTextEdit.

    cursor.endEditBlock();
    qDebug() << "MainWindow::filTextEdit(" << numLines << ") took " << et.elapsed() << "ms to complete.";
}
