 P3309 Зайцева Ирина Сергеевна \
 Базовый трек, оценка 3. ЛР 1. Shell (vfork) \
 Вариант: MacOS, search-name, bin-search


Оглавление
1. [Задание Часть 1](#задание-часть-1-запуск-программ)
    - [Текст задания](#текст-задания)
    - [Выполнение](#выполнение)
    - [Исходный код](#исходный-код)
    - [Пример работы](#пример-работы)
2. [Задание Часть 2](#задание-часть-2-мониторинг-и-профилирование)
    - [Текст задания](#текст-задания-1)
    - [Выполнение](#выполнение-1)
    - [Предварительная оценка](#предварительная-оценка)
    - [Бинарный поиск](#binsearch)
    - [Поиск по имени](#search-name)
    - [Объединенный нагрузчик](#multi-thread)


# Задание Часть 1. Запуск программ
## Текст задания 
Необходимо реализовать собственную оболочку командной строки - shell. Выбор ОС для реализации производится на усмотрение студента. Shell должен предоставлять пользователю возможность запускать программы на компьютере с переданными аргументами командной строки и после завершения программы показывать реальное время ее работы (подсчитать самостоятельно как «время завершения» – «время запуска»).
## Выполнение
1. Парсинг пользовательского ввода

    Для обработки ввода используется функция ParseInput, которая:

    Делит строку ввода на токены (аргументы).
    Возвращает результат в виде вектора строк.

    ```c++
    std::vector<std::string> ParseInput(const std::string& input) {
    std::vector<std::string> tokens;
    std::string item;
    std::stringstream ss(input);
    while (ss >> item) {
        tokens.push_back(item);
    }
    return tokens;
    }
    ```     
2. Выполнение команды

    Функция ExecuteCommand отвечает за:

    - Создание дочернего процесса с помощью vfork для запуска программы.
    - Использование execvp для выполнения команды.
    - Подсчет времени выполнения команды с использованием библиотеки <chrono>.
    - Обработку ошибок, таких как неудачный запуск дочернего процесса или ошибки выполнения команды.

    ``` c++
    void ExecuteCommand(const std::vector<std::string>& args) {
        if (args.empty()) {
            PrintError("Emptiness is not a command");
            return;
        }

        std::vector<char*> argv;
        for (const auto& arg : args) {
            argv.push_back(const_cast<char*>(arg.c_str()));
        }
        argv.push_back(nullptr);

        auto start = std::chrono::high_resolution_clock::now();

        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wdeprecated-declarations"
        pid_t pid = vfork();
        #pragma clang diagnostic pop

        if (pid < 0) {
            PrintError("vfork failed((!");
            return;
        }

        if (pid == 0) {
            execvp(argv[0], argv.data());
            PrintError("Execution failed((");
            _exit(1);
        }

        int status;
        waitpid(pid, &status, 0);

        std::chrono::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        PrintInfo("Program executed in  " + std::to_string(elapsed.count()) + " seconds.");
    }
    ```
### Исходный код
[Исходный код можно найти здесь](/source/monolith/)
### Пример работы
![alt text](image135.png)


# Задание Часть 2. Мониторинг и профилирование

## Текст задания
Разработать комплекс программ-нагрузчиков по варианту, заданному преподавателем. Каждый нагрузчик должен, как минимум, принимать параметр, который определяет количество повторений для алгоритма, указанного в задании. Программы должны нагружать вычислительную систему, дисковую подсистему или обе подсистемы сразу. Необходимо скомпилировать их без опций оптимизации компилятора.

Перед запуском нагрузчика, попробуйте оценить время работы вашей программы или ее результаты (если по варианту вам досталось измерение чего либо). Постарайтесь обосновать свои предположения. Предположение можно сделать, основываясь на свой опыт, знания ОС и характеристики используемого аппаратного обеспечения.

1. Запустите программу-нагрузчик и зафиксируйте метрики ее работы с помощью инструментов для профилирования. Сравните полученные результаты с ожидаемыми. Постарайтесь найти объяснение наблюдаемому.
2. Определите количество нагрузчиков, которое эффективно нагружает все ядра процессора на вашей системе. Как распределяются времена  USER%, SYS%, WAIT%, а также реальное время выполнения нагрузчика, какое количество переключений контекста (вынужденных и невынужденных) происходит при этом?
3. Увеличьте количество нагрузчиков вдвое, втрое, вчетверо. Как изменились времена, указанные на предыдущем шаге? Как ведет себя ваша система?
4. Объедините программы-нагрузчики в одну, реализованную при помощи потоков выполнения, чтобы один нагрузчик эффективно нагружал все ядра вашей системы. Как изменились времена для того же объема вычислений? Запустите одну, две, три таких программы.
5. Добавьте опции агрессивной оптимизации для компилятора. Как изменились времена? На сколько сократилось реальное время исполнения программы нагрузчика?

## Выполнение 
[Исходный код можно найти здесь](/benchmark/monolith/)

### Предварительная оценка
```bash
sysctl -a | grep cache

...
hw.perflevel0.l1icachesize: 196608
hw.perflevel0.l1dcachesize: 131072
hw.perflevel0.l2cachesize: 12582912
hw.perflevel1.l1icachesize: 131072
hw.perflevel1.l1dcachesize: 65536
hw.perflevel1.l2cachesize: 4194304
...
hw.cachelinesize: 128
```

- L1-кеш
    - L1 Data Cache (L1d):
        - Объём: 128 KB (для высокопроизводительных ядер hw.perflevel0.l1dcachesize) и 64 KB (для энергоэффективных ядер hw.perflevel1.l1dcachesize).
    - L1 Instruction Cache (L1i):
        - Объём: 192 KB (для высокопроизводительных ядер hw.perflevel0.l1icachesize) и 128 KB (для энергоэффективных hw.perflevel1.l1icachesize).
- L2-кеш
    - Объём: 12 MB для высокопроизводительных ядер (hw.perflevel0.l2cachesize) и 4 MB для энергоэффективных ядер (hw.perflevel1.l2cachesize).
- Линия кеша
    - Размер: 128 байт (hw.cachelinesize).
    - Линия кеша определяет количество данных, которые загружаются из памяти в кеш за один раз.

```bash
sudo powermetrics --samplers cpu_power
```

Хар-ки:
- Частота процессора:
    - Частота P-ядра (высокопроизводительных): до 3.2 GHz.
    - Частота E-ядра (энергоэффективных): около 2.0 GHz.
- Оперативная память (RAM):
    M1 имеет интегрированную память (Unified Memory Architecture, UMA), которая объединяет RAM и видеопамять (VRAM) в одном пуле: 
    - Объем памяти: 16 GB.
    - Тип памяти: LPDDR4X.
        - Low Power 
        - Double Data Rate (Данные передаются дважды за такт — один раз на фронте, второй раз на спаде сигнала)
        - 4X
    - Пропускная способность: до 68.25 GB/s.

## binsearch: 

Поиск элемента в отсортированном массиве целых чисел:	

```c++
bool binarySearch(const std::vector<int>& arr, int target) {
  int left = 0;
  int right = arr.size() - 1;

  while (left <= right) {
    int mid = left + (right - left) / 2;

    if (arr[mid] == target) {
      return true;
    }
    if (arr[mid] < target) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }
  return false;
}
```
```c++
static void BM_BinarySearch(benchmark::State& state) {
  const int size = 100000000;
  std::vector<int> data(size);
  std::iota(data.begin(), data.end(), 0);

  int target;

  for (auto _ : state) {
    target = rand() % size;
    binarySearch(data, target);
  }
}

BENCHMARK(BM_BinarySearch)->Iterations(1000);
BENCHMARK_MAIN();
```

Оценка алгоритма:
- время: O(log(n))
- память: О(n)
Для массива из N = 10 0000 000 элементов потребуется примерно log<sub>2</sub>(100 000 000) ≈ 27 сравнений.

Данные на 100 миллионов чисел занимают 
100 000 000 × 4 = 400 MB, что превышает размер L2-кеша (12 MB) и значительно превышает L1-кеш.
Это означает, что большая часть данных будет читаться из оперативной памяти, что значительно медленнее по сравнению с кешем.

T<sub>цикл</sub>= 1/3.2ГГц = 0.3125 нс \
T<sub>L1</sub>= 4 * 0.3125 нс = 1.25 нс

T<sub>L2</sub>= 15 * 0.3125 нс = 4.6875 нс


L1 кеш: ~4-5 циклов (∼1.25ns на 3.2 ГГц)\
L2 кеш: ~15-18 циклов (∼4.6875ns на 3.2 ГГц)\
RAM: ~50-120 ns

RAM : 27 * 50 = 1350 ns\
L1: 27 * 1.25 = 33.75 ns\
L2: 27 * 4.7 = 126.9 ns

Рассмотрели крайние случаи, по факту же будет, что обращение будет в разные места, поэтому эта оценка близка к истине.\
Реальное время выполнения без оптимизации:

![alt text](image-4.png)
---



производился запуск нагрузчика с разными флагами оптимизации 

**запуск без оптимизации (О0)**

Реальное время выполнения (Time) и процессорное время (CPU Time)  совпадают. Это указывает на минимальные потери из-за переключений контекста и других системных накладных расходов.Если бы были значительные задержки ввода-вывода (например, при чтении данных с диска), реальное время было бы больше CPU времени. Здесь этого не наблюдается.

![alt text](image-5.png)

**O2**

оптимизация ускроила на 1% (условно):

![alt text](image-6.png)

**O3**

-O3, будучи агрессивным, может ухудшить производительность из-за увеличения кода, усложнения инструкций и снижения эффективности использования кэша. Поэтому в данном случае прирост от -O3 либо отсутствует, либо приводит к небольшому замедлению.

![alt text](image-7.png)

**-O3 -march=native -funroll-loops -ffast-math**

Флаг -march=native позволяет использовать инструкции, специфичные для данной архитектуры CPU. Это могло бы дать преимущество, но такого не произошло, вероятно из-за того, что тестируемый код не сильно выигрывает от SIMD-инструкций или других оптимизаций.

![alt text](image-8.png)

---
- Основное ускорение достигается при переходе от -O0 к -O2. Флаги -O3 и специфичные оптимизации дают минимальные приросты для данного типа задач.
- Значение Load Average указывает на то, что система не полностью перегружена



после запуска нагрузчика производился мониторинг в Instrumants XCode для macOS 

---
На macOS используется архитектура big.LITTLE, которая комбинирует ядра с высокой производительностью (performance cores, P-ядра) и энергоэффективные ядра (efficiency cores, E-ядра). Разделение этих ядер можно наблюдать на скринах.

Это сделано для обеспечения высокой производительности при минимальном энергопотреблении. 
Efficiency Cores (E-ядра):
- Оптимизированы для выполнения фоновых задач, требующих низкого энергопотребления.
- Прекрасно подходят для задач с низкой интенсивностью вычислений, таких как обновление интерфейса, системные фоновые процессы и обработка простой логики.
\Performance Cores (P-ядра):
- Разработаны для максимальной производительности.
- Используются для ресурсоемких задач, таких как компиляция, обработка мультимедиа, игры или работа с большими данными.
---
## timeprofiler 
### запуск 1 экземпляра
![alt text](image-14.png)
![alt text](image-15.png)
![alt text](image-61.png)
![alt text](image-62.png)
На скринах показано, как используются ядра. Первые 4 ядра практически не берут на себя работу, т.к. это не их "профиль" и Performance Cores справляются. \
Ниже на первом скрине можно увидеть, какие функции занимают больше времени на выполнение.
### в какой-то момент запустили 2ой экземпляр
![alt text](image-16.png)
![alt text](image-17.png)
![ cr](image-63.png) скачаок соответствует включению 2ого экземпляра
![alt text](image-64.png)
На первом скрине можно наблюдать "провалы": насколько я поняла, это, возможно, переключение контекста. 

### 3 экземпляра 
![alt text](image-18.png)
![alt text](image-19.png)
![alt text](image-65.png)

Видимо, из-за неодновременного запуска нагрузчиков график имеет такие пробелы.

#### 4 экземпляра
![alt text](image-20.png)
![alt text](image-21.png)

#### 8 экземпляров
![alt text](image-22.png)
![alt text](image-23.png)

Захотелось зпускить 8 экземпляров, чтобы посмотреть, как на себя возьмут нагрузку E-ядра.

#### 13 экземпляров
![alt text](image-66.png)
как будто можно было еще запустить, но меня начали пугать скачки % системы и пользователя. При 10 нагрузчиках система имела около 10% и пользователь занимал около 85-90%

###
данные для графика получаем с помощь команды:

```bash
top -pid <PID> > process_output.txt &
sleep <n_sec> && kill $!
```

пример данных в файле process_output.txt:

![alt text](image-34.png)

3д график Time - CPU Usage (%) - Memory Usage (MB)

график по 100 точкам:
![alt text](image-35.png)

график по 300 точкам:
![alt text](image-36.png)

3д график Time - CPU Usage (%) - Disk (MB)
график по 300 точкам:
![alt text](image-37.png)

3д график Time - CPU Usage (%) - Load Avg
график по 300 точкам:
![alt text](image-38.png)




# search name 

Поиск файла в файловой системе по имени:

```C++
    bool searchFile(const fs::path& directory, const std::string& filename) {
    try {
        if (!fs::exists(directory)) {
        std::cerr << "Directory does not exist: " << directory << std::endl;
        return false;
        }

        if (!fs::is_directory(directory)) {
        std::cerr << "Path is not a directory: " << directory << std::endl;
        return false;
        }

        for (auto it = fs::recursive_directory_iterator(
                directory, fs::directory_options::skip_permission_denied
            );
            it != fs::recursive_directory_iterator();
            ++it) {
        std::error_code ec;

        if (it->is_directory(ec) && ec) {
            std::cerr << "Restricted directory: " << it->path() << " (" << ec.message() << ")"
                    << std::endl;
            it.disable_recursion_pending();
            continue;
        }

        if (it->is_fifo(ec) || it->is_other(ec) || it->is_socket(ec) || it->is_symlink(ec) ||
            it->is_character_file(ec) || ec) {
            std::cerr << "Strange file: " << it->path() << std::endl;
            continue;
        }

        try {
            if (fs::is_regular_file(*it, ec) && !ec && it->path().filename() == filename) {
            std::cout << "Found: " << it->path().string() << std::endl;
            return true;
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error accessing file: " << e.what() << std::endl;
            continue;
        }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing the directory: " << e.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
    std::cout << "File not found! " << std::endl;
    return false;
}
```

```C++
static void BM_SearchFile(benchmark::State& state) {
  const fs::path directory = "/Users/zirtoshka/inf_sys";
  const std::string filename = "target_file.txt";

  std::cout << "Benchmarking search in directory: " << directory << std::endl;


  for (auto _ : state) {
      searchFile(directory, filename);
  }
}

BENCHMARK(BM_SearchFile)->Iterations(100);
BENCHMARK_MAIN();
```
данный нагрузчик производит поиск по директории inf_sys (/Users/zirtoshka/inf_sys), для примерного расчета узнаем сколько файлов в inf_sys и всех её подкаталогах (245642):

![alt text](image-3.png)

Основное время требуется для операций на уровне файловой системы, которые проверяют метаданные файлов. В случае с M1, можно ожидать, что каждое такое действие может занять менее 1 миллисекунды в идеальных условиях, особенно для операций с метаданными (проверка типа файла и сравнение имени).

245642 * 1 мс ≈ 4 минуты ))

реальное время:
![alt text](image-1.png)

далее запуск с arg(100):

**без оптимизации**

![alt text](image-9.png)

**O2**

![alt text](image-10.png)

**O3**

![alt text](image-12.png)

**-O3 -march=native -funroll-loops -ffast-math**

![alt text](image-13.png)

- Время, затраченное на проверку файлов, не ускоряется значительно с оптимизациями, поскольку основная задержка приходит от чтения и обработки файлов.
- Оптимизации компилятора, такие как -funroll-loops и -march=native, могут не повлиять на производительность в случае с файловыми операциями, так как они в основном касаются вычислительных операций в коде, а не работы с файловой системой.

---

## timeprofiler
### запустили 1 экземпляра
![alt text](image-24.png)
![alt text](image-25.png)

### в какой-то момент запустили 2 экземпляра
![alt text](image-26.png)
![alt text](image-27.png)

### 3 экземпляра
![alt text](image-28.png)
![alt text](image-29.png)

#### 4 экземпляра
![alt text](image-30.png)
![alt text](image-31.png)

#### 8 экземпляров
![alt text](image-33.png)
![alt text](image-32.png)


Есть схожесть с предыдущим нагрузчиком в наличии переключении контекста и загруженности E-ядер по мере увеличения количества экземпляров.\
\
Неравномерный график для ядер с пиками и провалами при запуске нагрузчиков, возможно, обусловлен тем, что:
- используются операции ввода-вывода (I/O) для работы с файловой системой (I/O-bound).Процессор может простаивать, ожидая завершения операций чтения из файловой системы.
- из-за рекурсивного обхода каталогов нагрузка может значительно варьироваться
    - даже если каталогов мало, но в них много файлов, нагрузка будет высокой
    - если есть вложенные каталоги с правами, которые нельзя обойти (skip_permission_denied), процессоры могут простаивать из-за обработки ошибок.
- дополнительные проверки, такие как is_fifo, is_other, и обработка исключений, добавляют нерегулярные пики
- задачи с большим количеством I/O могут направляться на E-cores ядра, что вызывает провалы в производительности





3д график Time - CPU Usage (%) - Memory Usage (MB)
график по 300 точкам:
![alt text](image-39.png)

3д график Time - CPU Usage (%) - Disk (MB)
график по 300 точкам:
![alt text](image-40.png)

3д график Time - CPU Usage (%) - Load Avg
график по 300 точкам:
![alt text](image-41.png)


# multi thread 
арг(100)

1поточ

"-O0"
![alt text](image-47.png)
"-O2"
![alt text](image-59.png)
"-O3"
![alt text](image-44.png)
"-O3 -march=native -funroll-loops -ffast-math"
![alt text](image-60.png)

---

2xпоточ

"-O0"
![alt text](image-52.png)

"-O2"

"-O3"

"-O3 -march=native -funroll-loops -ffast-math"


4xпоточ

"-O0"
![alt text](image-58.png)
"-O2"

"-O3"

"-O3 -march=native -funroll-loops -ffast-math"


Многопоточные задачи менее чувствительны к флагам оптимизации, так как время выполнения ограничено синхронизацией и распределением потоков. Небольшое ухудшение времени, возможно, связано с более сложным управлением потоками или снижением точности при агрессивной оптимизации.

## allocations
### запустили однопоточ
![alt text](image-45.png)
![alt text](image-46.png)

## timeprofiler
### запустили однопоточ
![alt text](image-42.png)
![alt text](image-43.png)
### 2хпоточ
![alt text](image-48.png)
![alt text](image-49.png)
![alt text](image-50.png)
![alt text](image-51.png)
### 4хпотточ
![alt text](image-55.png)
![alt text](image-53.png)
![alt text](image-54.png)
![alt text](image-56.png)
![alt text](image-57.png)
