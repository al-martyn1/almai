# Тест разбора markdown на отдельные файлы

**C:\dummy.txt**
```
Пытаемся записать C:\dummy.txt
```

**\\server\C$\dummy.txt**
```
Пытаемся записать \\server\C$\dummy.txt
```

**almai-md-split.test01\..\..\dummy.txt**
```
Пытаемся записать almai-md-split.test01\..\..\dummy.txt
```

````
#include <stdio.h>
// Файл без названия
int main()
{
    printf("hello 0\n");
}
````

**dummy.txt**
**test.txt**
```
Просто тестовый файл
```

Плюсовый сорц
**test.cpp**
```
// Плюсовый файл 1 (с одной строкой коментария)
```

**test.cpp**
```
// Плюсовый файл 2. Должен быть конфликт имён и автонумерация второго файла
```

Markdown
~~~markdown
# Markdown тест

Листинг в маркдауне
```cpp
#include <stdio.h>
int main()
{
    printf("hello 1\n");
}
```

Табулированный (пробелами) листинг
    #include <stdio.h>
    int main()
    {
        printf("hello 2\n");
    }

Ещё листинг - без названия

````
#include <stdio.h>
// Файл без названия
int main()
{
    printf("hello 3\n");
}
````

~~~

Табулированный (пробелами) листинг
    #include <stdio.h>
    int main()
    {
        printf("hello 2\n");
    }

Ещё листинг - без названия

````
#include <stdio.h>
// Файл без названия
int main()
{
    printf("hello 3\n");
}
````



Маркер языка
````C++
#include <stdio.h>
int main()
{
    printf("hello 4\n");
}
````

Хвостовой маркер языка
````
#include <stdio.h>
int main()
{
    printf("hello 5\n");
}
````C++

Просто текст.
