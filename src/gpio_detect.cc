#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    int gpio_pin = 55;

    FILE *export_file = fopen("/sys/class/gpio/export", "w");
    if (export_file == NULL)
    {
        perror("Failed to open GPIO export file");
        return -1;
    }
    fprintf(export_file, "%d", gpio_pin);
    fclose(export_file);

    char direction_path[50];
    snprintf(direction_path, sizeof(direction_path), "/sys/class/gpio/gpio%d/direction", gpio_pin);
    FILE *direction_file = fopen(direction_path, "w");
    if (direction_file == NULL)
    {
        perror("Failed to open GPIO direction file");
        return -1;
    }
    fprintf(direction_file, "in");
    fclose(direction_file);

    char value_path[50];
    char cat_command[100];
    snprintf(value_path, sizeof(value_path), "/sys/class/gpio/gpio%d/value", gpio_pin);
    snprintf(cat_command, sizeof(cat_command), "cat %s", value_path);
    system(cat_command);

    return 0;
}