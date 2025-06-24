/*!
* \file
* \brief Данный файл содержит главную функцию программы textExplanationsInRu.
*
* \mainpage Документация для программы "text explanations in Russian language (textExplanationsInRu)"
Программа предназначена для генерации текстового объяснения выражения на русском языке. Она принимает на вход XML-файл с описанием выражения и генерирует соответствующее объяснение в виде текстового файла.
\n\nДля функционирования программы необходима операционная система Windows 7 или выше.
\nТребуемые библиотеки: Qt6Core.dll, Qt6Xml.dll, libgcc_s_seh-1.dll, libstdc++-6.dll, libwinpthread-1.dll
\nПрограмма должна получать два аргумента командной строки: имя входного файла и имя выходного файла в формате 'txt'

\nПример команды запуска программы:
* \code
.\textExplanationsInRu.exe input.txt output.txt
* \endcode

* \author Popova Anna
* \date June 2025
* \version 1.0
*/

#include "expression.h"
#include "teexception.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <windows.h>
#include <conio.h>
#include <QTextStream>


/*!
 * \brief Выводит справочное сообщение в поток
 * \param[out] cout Поток вывода (например, QTextStream(stdout))
 * \param[in] filename Имя исполняемого файла, подставляется в шаблон использования
 */
void printHelpMessage(QTextStream& cout, const QString& filename);

/*!
 * \brief Печатает пояснение выражения, считанного из XML-файла
 * \param[out] cout Поток, в который выводится пояснение
 * \param[in] inputFile Путь к входному XML-файлу с выражением
 * \param[in] outputFile Путь к выходному файлу (если необходимо сохранить результат)
 */
void printExplanation(QTextStream& cout, const QString& inputFile, const QString& outputFile);

/*!
 * \brief Проверяет доступность файла для записи
 * \param[in] filePath Путь к файлу, который нужно проверить
 * \throws TEException Если файл не может быть создан или открыт для записи
 */
void checkFileAccess(const QString& filePath);

/*!
 * \brief Записывает текст в указанный файл
 * \param[in] filePath Путь к файлу, в который нужно записать содержимое
 * \param[in] content Содержимое, которое будет записано в файл
 * \throws TEException Если файл не может быть открыт для записи
 */
void writeToFile(const QString& filePath, const QString& content);


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
