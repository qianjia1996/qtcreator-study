
#include <QApplication>
#include <QtCore>
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QTextCodec::setCodecForTr( QTextCodec::codecForName("gb2312"));	
    ThreadDlg dialog;
    dialog.show();
    return dialog.exec();
}
