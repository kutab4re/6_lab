#include <iostream>
#include <vector>
#include <climits>
#include <string>

using namespace std;
 
int k = 0; // переменная для подсчёта итераций
int T = 0; // переменная для общей длины пути
vector<vector<int>> way; // матрица для хранения длины путей

void change_elem(vector<vector<int>>& matrix, vector<vector<int>>& Mflag) {
    int s = 0; // Переменная для хранения суммы вычитаемых элементов
    // Проход по строкам матрицы
    for (int i = 0; i < matrix.size(); i++) {
        bool flag = false;
        for (int r = 0; r < matrix.size(); r++)
            if (Mflag[i][r] != 2) {
                flag = true; // Если строка не вычеркнута, устанавливаем флаг в true
                break;
            }
        if (flag) {
            int min = INT_MAX; // Переменная для хранения минимального значения в строке
            for (int j = 0; j < matrix.size(); j++)
                if (matrix[i][j] < min && Mflag[i][j] == 0)
                    min = matrix[i][j]; // Находим минимальный невычеркнутый элемент в строке

            s += min; // Добавляем минимальное значение к сумме

            for (int j = 0; j < matrix.size(); j++)
                if (Mflag[i][j] == 0)
                    matrix[i][j] -= min; // Вычитаем минимальное значение из невычеркнутых элементов строки
        }
    }
    // Проход по столбцам матрицы
    for (int i = 0; i < matrix.size(); i++) {
        bool flag = false;
        for (int r = 0; r < matrix.size(); r++)
            if (Mflag[r][i] != 2) {
                flag = true; // Если столбец не вычеркнут, устанавливаем флаг в true
                break;
            }
        if (flag) {
            int min = INT_MAX; // Переменная для хранения минимального значения в столбце
            for (int j = 0; j < matrix.size(); j++)
                if (matrix[j][i] < min && Mflag[j][i] == 0)
                    min = matrix[j][i]; // Находим минимальный невычеркнутый элемент в столбце

            s += min; // Добавляем минимальное значение к сумме

            for (int j = 0; j < matrix.size(); j++)
                if (Mflag[j][i] == 0)
                    matrix[j][i] -= min; // Вычитаем минимальное значение из невычеркнутых элементов столбца
        }
    }
    T += s; // Добавляем сумму вычитанных элементов к общей длине пути
    cout << endl;
}

int* grade(vector<vector<int>>& matrix, vector<vector<int>>& flag) {
    static int zero[2] = { -1, -1 }; // Массив для хранения координат выбранного нуля
    double max = 0; // Переменная для хранения максимальной оценки
    // Проход по всем элементам матрицы
    for (int i = 0; i < matrix.size(); i++)
        for (int j = 0; j < matrix.size(); j++)
            if (matrix[i][j] == 0 && flag[i][j] == 0) { // Если элемент равен нулю и не вычеркнут
                double min_i = INT_MAX, min_j = INT_MAX; // Переменные для хранения минимальных значений в строке и столбце
                // Находим минимальный невычеркнутый элемент в столбце
                for (int k = 0; k < matrix.size(); k++) {
                    if (k != i && matrix[k][j] < min_j && flag[k][j] == 0)
                        min_j = matrix[k][j];
                }
                // Находим минимальный невычеркнутый элемент в строке
                for (int k = 0; k < matrix.size(); k++) {
                    if (k != j && matrix[i][k] < min_i && flag[i][k] == 0)
                        min_i = matrix[i][k];
                }
                if (max < min_i + min_j) { // Если оценка для текущего нуля больше максимальной
                    max = min_i + min_j; // Обновляем максимальную оценку
                    zero[0] = i; // Запоминаем координаты текущего нуля
                    zero[1] = j;
                }
            }
    return zero; // Возвращаем координаты выбранного нуля
}

void komivoyadjer(vector<vector<int>> matr, vector<vector<int>> Mflag) {
    k++; // Увеличиваем счетчик шагов

    change_elem(matr, Mflag); // Выполняем приведение матрицы

    int* zero = grade(matr, Mflag); // Находим нулевой элемент с максимальной оценкой
    way[zero[0]][zero[1]] = 1; // Помечаем выбранное ребро в путь
    if (Mflag[zero[1]][zero[0]] != 2)
        Mflag[zero[1]][zero[0]] = 1; // Если элемент еще не вычеркнут, вычеркиваем его

    cout << "Выбрано ребро: " << zero[0] + 1 << ", " << zero[1] + 1 << endl;

    for (int i = 0; i < matr.size(); i++) {
        Mflag[zero[0]][i] = 2; // Вычеркиваем строку, связанную с выбранным элементом
        Mflag[i][zero[1]] = 2; // Вычеркиваем столбец, связанный с выбранным элементом
    }

    // Вывод промежуточной приведенной матрицы
    for (int i = 0; i < matr.size(); i++) {
        bool flag = false;
        for (int r = 0; r < matr.size(); r++)
            if (Mflag[i][r] != 2) {
                flag = true;
                break;
            }

        if (flag) {
            for (int j = 0; j < matr.size(); j++) {
                if (Mflag[i][j] == 1)
                    cout << "- "; // Выводим прочерк для вычеркнутых элементов
                if (Mflag[i][j] == 0)
                    cout << matr[i][j] << " "; // Выводим значение элемента
            }
            cout << endl;
        }
    }
    cout << endl;

    if (k < matr.size())
        komivoyadjer(matr, Mflag); // Рекурсивно вызываем функцию для следующего шага
}

int main() {
    setlocale(LC_ALL, "ru");
    int n;
    cout << "Введите размерность матрицы: ";
    cin >> n;

    // Инициализация матрицы расстояний между городами
    vector<vector<int>> matrix(n, vector<int>(n));
    cout << "Введите матрицу расстояний:" << endl;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> matrix[i][j];

    cout << "\n/----------------------------------------------------------------/\n\n";

    // Матрица флагов
    vector<vector<int>> flag(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        flag[i][i] = 1;  // На диагонали - прочерки

    // Инициализация вектора Way
    way.resize(n, vector<int>(n, 0));

    // Вывод матрицы расстояний
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            cout << (i == j ? "- " : to_string(matrix[i][j]) + " ");
        cout << endl;
    }

    // Вызов основной функции для решения задачи коммивояжера
    komivoyadjer(matrix, flag);

    // Вывод результата
    cout << "Весь путь: 1";
    int l = 0;
    for (int i = 0; i < n; i++) {
        int j;
        for (j = 0; j < n; j++)
            if (way[l][j] == 1) {
                cout << j + 1;
                l = j;
                break;
            }
    }
    cout << endl;
    cout << "Общая длина пути: " << T << endl;

    return 0;
}

