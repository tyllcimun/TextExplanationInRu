#include "expression.h"
#include "teexception.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <windows.h>
#include <conio.h>
#include <QTextStream>



void printHelpMessage(QTextStream& cout, const QString& filename);
// void printTests(QTextStream& cout);
void printExpressionInfo(QTextStream& cout, const QString& inputFile, const QString& outputFile);
void printExplanation(QTextStream& cout, const QString& inputFile, const QString& outputFile);


int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    QTextStream cout(stdout);
    cout.setEncoding(QStringConverter::Utf8);

    QCoreApplication a(argc, argv);

#if defined(TESTS)
    return runTest(argc, argv);
#endif

    QString fileName = QCoreApplication::applicationFilePath();
    QFileInfo fileInfo(fileName);
    fileName = fileInfo.fileName();

    // Если первый аргумент "-help"
    if(QString(argv[1]) == "-help") {
        // Напечатать справочную информацию
        printHelpMessage(cout, fileName);
    }
    // Если первый аргумент "-test"
    else if(QString(argv[1]) == "-test") {
        // Выполнить тесты
    }
    // Если аргумента три и второй не начинается с "-"
    else if(argc == 3 && !QString(argv[2]).startsWith("-")) {
        printExplanation(cout, argv[1], argv[2]);
    }
    else {
        cout << ("Ошибка в синтаксисе команды. Подробнее: .\\" + fileName +  " -help");
    }

    a.exit(0);
    return 0;
}

void checkFileAccess(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw TEException(ErrorType::OutputFileCannotBeCreated, QList<QString>{filePath});
    }
    file.close();
}

void writeToFile(const QString& filePath, const QString& content) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
    } else {
        throw TEException(ErrorType::OutputFileCannotBeCreated, QList<QString>{filePath});
    }
}

void printExplanation(QTextStream& cout, const QString& inputFile, const QString& outputFile) {
    try {
        // Проверить доступ к выходному файлу
        checkFileAccess(outputFile);
        // Считать входной файл
        Expression exp = Expression::fromFile(inputFile);
        // Получить объяснение выражения
        QString explanation = exp.getExplanationInRu();
        // Вывести объяснение в консоль
        cout << explanation;
        // Записать объяснение в выходной файл
        writeToFile(outputFile, explanation);
    } catch (QList<TEException>& errors) {
        for (const TEException& error : errors) {
            cout << error.what() << "\n";
        }
    } catch (TEException& error) {
        cout << error.what();
    }
}

void printExpressionInfo(QTextStream& cout, const QString& inputFile, const QString& outputFile){

    try {
        Expression expr = Expression(inputFile);
        cout << expr.ToQstring();
    }
    catch(TEException exception) {
        cout << exception.what();
    }
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
