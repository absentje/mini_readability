# mini_readability
Задача - Mini readability. Выполнение на C++. В среде Visual Studio 2015.
Задача - «вытащить» из веб-страницы только полезную информацию, отбросив весь «мусор» (навигацию, рекламу и тд).
Полученный текст нужно отформатировать для максимально комфортного чтения в любом текстовом редакторе. Правила форматирования: ширина строки не больше 80 символов (если больше, переносим по словам), абзацы и заголовки отбиваются пустой строкой. Если в тексте встречаются ссылки, то URL вставить в текст в квадратных скобках. Программа оформляется в виде утилиты командной строки, которой в качестве параметра указывается произвольный URL. Она извлекает по этому URL страницу, обрабатывает ее и формирует текстовый файл с текстом статьи, представленной на данной странице. + некоторые усложнения, описанные ниже.
-------------------------------------------------------------------------------------------------------------------
Перед началом работы необходимо было установить библиотеку boost(в частности нужен был
boost::asio) и OpenSSL (для работы с https протоколом).

Класс MakeFullHtmlFile служит для получения полного html файла заданного url. Он включает 2 метода, соответствующие http и https подключениям(то есть пользователю необходимо указать протокол). В доработке программы можно, получая информацию о подключении к протоколу http обрабатывать коды состояния и перенаправлять(если нужно) и исправлять другие ошибки подключения. Полный html файл записывается в файл по примеру:
lenta.ru/news/2015/12/15/american - lenta.ru+news+2015+12+15+american_FULL_HTML.txt

Класс Tag содержит в себе:
1) ссылку на string, в котором содержится полный html файл;
2) полное имя тега, сокращенное имя тега;
3) позиции начала и конца содержимого тега(позиции в полном html файле;
4) list<Tag> - список потомков(1-го рода);
5) указатель на родителя.

Алгоритм построения иерархии тегов заключается в рекурсивном создании тегов. При создании тега мы знаем лишь позицию начала его содержимого - отталкиваясь от этого ищем открывающийся или закрывающийся тег. Если наткнулись на закрыв. - то данный тег заканчивается там, создание тега закончено. Если же наткнулись на открыв. - то создаем новый тег и помещаем его в список потомков, продолжаем дальнейший поиск после конца содержимого этого потомка.

После того как построили иерархию, необходимо оставить только необходимые теги. Для этого есть метод getTags, принимающий ссылку на string(куда необходимо записать текст с данными тегами) и set<string>& tags(множество тегов, set - для быстрого поиска в контейнере). Вывод происходит опять же рекурсивно. Если название данного тега(this) есть в set'е, то выводим его. Если же не содержится, то проверяет поочередно каждого из сыновей в list'е.(вызывая эту же функцию для каждого итератора list'а)

Класс HtmlParser служит неким интерфейсом для работы с Tag'ом.
HtmlParser содержит:
1)shared_ptr<Tag> file; - указательна иерархию всего html файла
(shared_ptr здесь для того чтобы лишний раз не вызывать конструктор копирования,
в конструкторе);
2) название файла с сайтом, вводимого пользователем;
3) string полного html файла.

HtmlParser имеет методы:
1)getTags(set<string>&) - возвращает строку с html файлом, с оставленными тегами
из set'а;
2)getClearText() - возвращает строку, с оставленными тегами(title p), абзацы отделены
отдельной строкой, ширина строки не больше 80 символов, в квадратные скобки вымещены
ссылки из тегов <a>, предусмотрены некоторые из символов-мнемоники(кавычки и тире).
3)прочие методы, конкретно для записи в файл.

Дальнейшее улучшение программы:
1)Как я и сказал вначале: в доработке программы можно, получая информацию о подключении к протоколу http обрабатывать коды состояния и перенаправлять(если нужно) и исправлять другие ошибки подключения;
2)Учесть большее кол-во символов-мнемоники(Character entity), а так же другие особенности разметки HTML;
3)Написать графический интерфейс(GUI) для данной программы, используя Qt.

Проверка произвелась на двух сайтах, примеры:(рез-т работы программы в папке examples в txt-файлах)
lenta.ru/news/2015/12/15/american
www.gazeta.ru/army/2017/02/28/10549073
