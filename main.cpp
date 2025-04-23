#include "expression.h"
#include "teexception.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <windows.h>
#include <conio.h>
#include <QTextStream>



void printHelpMessage(QTextStream& cout, const QString& filename);
void printExpanssion(QTextStream& cout, const QString& inputFile, const QString& outputFile);
void printExplanation(QTextStream& cout, const QString& inputFile, const QString& outputFile);


int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    QCoreApplication a(argc, argv);
    QTextStream cout(stdout);
    // cout.setEncoding(QStringConverter::Utf8);

    QString fileName = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(fileName);
    fileName = fileInfo.fileName();

    // Если первый аргумент "-help"
    if(QString(argv[1]) == "-help") {
        // Напечатать справочную информацию
        printHelpMessage(cout, fileName);
    }
    // Если аргумента три и второй не начинается с "-"
    else if(argc == 3 && !QString(argv[2]).startsWith("-")) {
        // Получить объяснение выражения
        printExplanation(cout, argv[1], argv[2]);
    }
    else {
        cout << ("Ошибка в синтаксисе команды. Подробнее: .\\" + fileName +  " -help");
    }

    a.exit(0);
    return 0;
}


void printExplanation(QTextStream& cout, const QString& inputFile, const QString& outputFile){
}
void printHelpMessage(QTextStream& cout, const QString& filename)
{
    cout << ".\\" + filename + " [-help | -test] [input-file] [output-file]\n";
    cout << "-help      - Выводит сообщение-помощник. При вводе этой команды путь к файлам указывать не нужно.\n";
    cout << "-test      - Запускает тесты. При вводе этой команды путь к файлам указывать не нужно.\n";
    cout << "input-file - путь к входному файлу. В случае, если в пути файла присутствуют пробелы, необходимо указать путь в кавычках. Например:\n";
    cout << "               \"C:\\\\input files\\input.txt\"\n";
    cout << "output-file - путь к выходному файлу. Если файла не существует - он будет создан. В случае, если в пути файла присутствуют пробелы, необходимо указать путь в кавычках. Например:\n";
    cout << "               \"C:\\\\output files\\output.txt\"\n";
    cout << "Пример запуска: \n";
    cout << "   .\\" + filename + " input.txt \"C:\\\\files\\New folder\\output.txt\"\n";
}
