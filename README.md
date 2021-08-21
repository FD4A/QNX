#### всякие полезности под QNX
**OC:** QNX
***
****RM_template****

Шаблон однопоточного менеджера ресурсов на диспатчере. Переопеределены структуры IOFUNC_ATTR_T и IOFUNC_OCB_T.
Переопределены функции open, devctl, close_ocb, notify. Определён таймер, менеджер ресурсов делает notify по
этому таймеру. Перехватываются _PULSE_CODE_COIDDEATH - оповещения от ядра о том что процессы нами открытые,
локальные или удалённые прекратили свою работу. Написан пример работы с источником данных в отдельном потоке. 
Отдельный поток для работы с источником данных нужен так как при переоткрытии через сеть будет сетевой таймаут, 
что может привести к некоректной работе (задержке всего выполнения на этот таймаут и соответствующий сайд эффекты).

****devctl_test****

Утилита которая помогает проверить корректоность работы функции io_devctl в RM_template.
Принимает три аргумента 1) путь до менеджера ресурсов, 2) номер devctl команды (десятичный!), 
3) int значение передаваемое командой. Запустить:

    ./devctl_test /dev/RM_template 1 111
    
Можно увидеть успешную передачу (в RM_template приём соответственно). Запустить:

    ./devctl_test /dev/RM_template 2 111
    
Можно увидеть I/O ошибку.

****notify_test****

Утилита которая позволяет проверить работу функции io_notify в RM_template.

    ./notify_test /dev/RM_template

Если доступа к RM_template не будет, то прекратит свою работу.

****notify_source****
Небольшой менеджер ресурсов, который делает notify по таймеру. 

    ./notify_source /dev/notify_source1
    ./notify_source /dev/notify_source2

Создадутся два ресурса, по указанным путям, которые подхватяться RM_template.
