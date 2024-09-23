#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Mi licencia");
MODULE_AUTHOR("Mauricio Luque Jiménez");
MODULE_DESCRIPTION("Un pequeño módulo personal desarrollado para este seminario");
MODULE_VERSION("1.0");

static char *autor = "Mauricio Luque Jiménez";
static char *version = "1.0";

module_param(autor, charp, S_IRUGO);
module_param(version, charp, S_IRUGO);

MODULE_PARM_DESC(autor, "El autor de este módulo de kernel");
MODULE_PARM_DESC(version, "La versión del módulo");

/**
 * @brief Función de inicialización del módulo.
 * @return 0 si el módulo se inicia correctamente 
 */
static int __init init_modulo(void)
{
    printk(KERN_INFO "El módulo, de versión %s, desarrollado por %s ha sido cargado en el kernel", version, autor);
    return 0;
} 

/**
 * @brief Función de finalización del módulo.
 */
static void __exit exit_modulo(void) { printk(KERN_INFO "El módulo ha sido eliminado del kernel"); }

module_init(init_modulo);
module_exit(exit_modulo);
