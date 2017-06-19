#include <QApplication>
#include "test/testrunner.h"
#include "mainwindow.h"


int main(int argc, char *argv[])
{
        test::test_runner const& runner = test::load(argc, argv);
        runner.run_all();

        QApplication a(argc, argv);
        MainWindow w;
        w.show();

        return a.exec();
}
