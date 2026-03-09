# Тест разбора markdown на отдельные файлы

**dummy.txt**
**test.txt**
```
Просто тестовый файл
```

Плюсовый сорц
**test.cpp**
```
// Просто тестовый файл
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
        printf("hello 1\n");
    }

Ещё

````
#include <stdio.h>
int main()
{
    printf("hello 1\n");
}
````

~~~
Хвостовой маркер языка
````
#include <stdio.h>
int main()
{
    printf("hello 1\n");
}
````C++

Просто текст.
