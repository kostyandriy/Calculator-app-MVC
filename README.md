<!DOCTYPE html>
<html>

<body>
    <h1>Руководство SmartCalc v2.0.</h1>
    <h2>Содержание</h2>
    <ol>
        <li><a name="1" href="#">Установка</a></li>
        <li><a name="2" href="#2-2">Запуск калькулятора</a></li>
        <li><a name="3" href="#3-3">Создание архива</a></li>
        <li><a name="4" href="#4-4">Описание SmartCalc_v2.0</a>
            <div class="description">
                <ol>
                    <li><a name="4.1" href="#4-4-1">Калькулятор</a></li>
                    <li><a name="4.2" href="#4-4-2">Построение графиков</a></li>
                    <li><a name="4.3" href="#4-4-3">Кредитный калькулятор</a></li>
                </ol>
            </div>
        </li>
        <li><a name="5" href="#5-5">Удаление</a></li>
    </ol>
    <h2><a name="1-1"></a>Установка</h2>
    <p>Чтобы установить калькулятор перейдите в терминале в папку src и выполните команду:</p>
    <pre>make install</pre>
    <p>Установленный калькулятор будет находиться в папке build под названием <b>SmartCalc2_0</b></p>
    <h2><a name="2-2"></a>Запуск калькулятора</h2>
    <p>Для использования/запуска калькулятора перейдите в терминале в папку build и выполните команду:</p>
    <pre>./SmartCalc2_0</pre>
    <h2><a name="3-3"></a>Создание архива</h2>
    <p>Чтобы создать архив калькулятора перейдите в терминале в папку src и выполните команду:</p>
    <pre>make dist</pre>
    <p>В папке src появится файл <b>SmartCalc2_0.tar.gz</b></p>
    <h2><a name="4-4"></a>Описание SmartCalc_v2.0</h2>
    <ul>
        <li><a name="4-4-1"></a>Калькулятор
            <p>При запуске калькулятора открывается главное окно - собственно калькулятор:</p>
            <img src="images/calc.png">
            <ul>
                <li>Калькулятор позволяет проводить арифметические операции с различными операторами (сложение,
                    вычитание, умножение, деление, остаток от деления, возведение в степень) и функциями (см. далее).
                </li>
                <li>На вход программы могут подаваться как целые числа, так и вещественные числа, записанные через
                    точку.</li>
                <li>Представляется возможность вводить выражение напрямую с клавиатуры.</li>
                <li>Производится вычисление произвольных скобочных арифметических выражений в инфиксной нотации
                </li>
                <li>Вычисление производятся после полного ввода вычисляемого выражения и нажатия на символ `=`.</li>
                <li>Предоставляется лимит выражения в 256 символов</li>
                <li>Функции:
                    <table>
                        <tr>
                            <th>Описание функции</th>
                            <th>Функция</th>
                        </tr>
                        <tr>
                            <td>Вычисляет косинус </td>
                            <td>cos(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет синус </td>
                            <td>sin(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет тангенс </td>
                            <td>tan(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет арккосинус </td>
                            <td>acos(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет арксинус </td>
                            <td>asin(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет арктангенс </td>
                            <td>atan(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет квадратный корень </td>
                            <td>sqrt(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет натуральный логарифм </td>
                            <td>ln(x)</td>
                        </tr>
                        <tr>
                            <td>Вычисляет десятичный логарифм </td>
                            <td>log(x)</td>
                        </tr>
                    </table>
                </li>
            </ul>
        </li>
        <li><a name="4-4-2"></a>Построение графиков
            <p>Чтобы перейти в окно построения графиков нужно ввести валидное выражение в инфиксной нотации в главном окне(калькуляторе) и нажать на кнопку "График", откроется следующее окно:</p>
            <img src="images/plot.png">
            <p>Доступен следующий функционал:</p>
            <ul>
                <li>Построение графика функции, заданной с помощью выражения в инфиксной нотации с переменной <b>x</b>.
                </li>
                <li> Построение графика, заданного с помощью выражения в инфиксной нотации без переменной (будет прямая)</li>
                <li>Задание области определения и области значения функции в диапазонах от -1000000 до 1000000</li>
            </ul>
        </li>
        <li><a name="4-4-3"></a>Кредитный калькулятор
            <ul>
                <p>При нажатии кнопки "Кредит" появляется следующее окно:</p>
                <img src="images/credit.png">
                <p>Доступен следующий функционал:</p>
                <li>Расчёт ежемесячного платежа, переплаты по кредиту и общей выплаты исходя из заданных значений общей
                    суммы кредита, срока и процентной ставки</li>
                <li>Выбор типа платежа: аннуитентный или дифференцированный</li>
            </ul>
        </li>
    </ul>
    <h2><a name="5-5"></a>Удаление</h2>
    <p>Чтобы удалить калькулятор перейдите в терминале в папку src и выполните команду:</p>
    <pre>make uninstall</pre>
</body>

</html>