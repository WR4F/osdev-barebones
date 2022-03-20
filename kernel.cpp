
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif


/** Memory allocation **/
// https://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf

#define ALIGNMENT 8// must be a power of 2
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define SIZE_T_SIZE (ALIGN(sizeof(size_t))) // header size


#undef errno
extern int errno;

#define STACK_BUFFER 65536 /* Reserved stack space in bytes. */

// void *sbrk (int nbytes) {
// 	/* Symbol defined by linker map */
// 	extern int end; /* start of free memory (as symbol) */

// 	/* Value set by crt0.S */
// 	extern void *stack; /* end of free memory */

// 	/* The statically held previous end of the heap, with its initialization. */
// 	static void *heap_ptr = (void*)&end; /* Previous end */

// 	if ((stack - (heap_ptr + nbytes)) > STACK_BUFFER ) {
// 		void *base = heap_ptr;
// 		heap_ptr += nbytes;
// 		return base;
// 	} else {
// 		errno = ENOMEM;
// 		return (void *) -1;
// 	}
// }

// // super-naive allocator
// void *malloc(size_t size) {
// 	size_t blk_size = ALIGN(size + SIZE_T_SIZE);
// 	size_t *header  = sbrk(blk_size);
// 	*header = blk_size | 1; // mark allocated bit
// 	return (char *)header + SIZE_T_SIZE;
// }

// void free(void *ptr) {
// 	size_t *header = (char *)ptr - SIZE_T_SIZE;
// 	*header = *header & ~1L; // unmark allocated bit
// }


/** Terminal Text I/O **/

// Hardware text mode color constants
enum vga_color {
	VGA_COLOR_UNSET = -1,
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

// Return foreground and background colors that map to correct VGA color code
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

// Return text and VGA color code mapping
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

// Max size of terminal
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Cursor (x,y) position
size_t terminal_row = 0;
size_t terminal_column = 0;

// Terminal buffer contents
uint16_t* terminal_buffer;

// Background colors, mapped to defaults
enum vga_color global_fg = VGA_COLOR_LIGHT_GREEN;
enum vga_color global_bg = VGA_COLOR_DARK_GREY;
uint8_t terminal_color;

// Initialize terminal and set buffer contents to blank values
void kernel_initialize(void)
{
	terminal_buffer = (uint16_t*) 0xB8000; // Location of video memory, holds 2 bytes for each cell on the screen
	terminal_color = vga_entry_color(global_fg, global_bg); // First byte is foreground color, second byte is background color
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

// Get length of string
size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

// Concatenate two strings
char* strcat(char* dest, const char* src)
{
	char* combined_str = dest;

	while (*dest)
		dest++;
	while ((*dest++ = *src++))
	;
	return combined_str;
}

// Changes terminal color global variables
void kernel_setcolor(enum vga_color fg, enum vga_color bg)
{
	global_fg = fg;
	global_bg = bg;
	terminal_color = vga_entry_color(global_fg, global_bg);
}

// Place character at (x,y) pos with specific color
void kernel_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

// Print or render a single character
void kernel_putchar(char c)
{
	// Deal with special character behavior
	switch (c)
	{
	case '\n':
		terminal_column = 0;
		terminal_row++;
		// scrolldown();
		return;
		break;
	default:
		break;
	}

	// Print regular character
	kernel_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
			// scrolldown();
	}
}

// Print character
char* kernel_appendchar(const char* str, const char* c)
{
	char* str_with_newline = (char*)str; // i think i need malloc and free for proper use
	return strcat(str_with_newline, c);
}

// Print a string
void kernel_writestring(const char* str)
{
	const size_t str_size = strlen(str);
	for (size_t i = 0; i < str_size; i++)
		kernel_putchar(str[i]);
}

// Print a string, end with newline
void kernel_writestringln(const char* str)
{
	// char* str_with_newline = kernel_appendchar(str, "\n");
	kernel_writestring(str);
	kernel_putchar('\n');
}

// Print a string in color
void kernel_writestring_color(const char* str, enum vga_color fg, enum vga_color bg)
{
	// Use default colors if fg and bg are unset and switch to default
	if (fg == VGA_COLOR_UNSET)
		fg = global_fg;
	if (bg == VGA_COLOR_UNSET)
		bg = global_bg;

	// Store temp color value, restore later
	const uint8_t terminal_color_tmp = terminal_color;
	terminal_color = vga_entry_color(fg, bg);

	kernel_writestring(str);
	terminal_color = terminal_color_tmp;
}

// Print a string in color, end with newline
void kernel_writestringln_color(const char* str, enum vga_color fg, enum vga_color bg)
{
	// char* str_with_newline = kernel_appendchar(str, '\n');
	kernel_writestring_color(str, fg, bg);
	kernel_putchar('\n');
}



/** Main Functions **/

void shell_loop() {
	// Default colors
	while (true)
	{
		char* str1 = (char*)"Hello World! ...2 + 2";
		const char* str2 = " = 4";
		const char* str3 = strcat(str1, str2);
		kernel_writestring(str3);
		kernel_writestring_color(str3, VGA_COLOR_UNSET, VGA_COLOR_UNSET);
		kernel_writestringln_color(str3, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
		kernel_writestringln(str3);
		kernel_setcolor(VGA_COLOR_BLACK, VGA_COLOR_BLUE);
		kernel_writestring(str3);
		kernel_writestringln_color(str3, VGA_COLOR_BLACK, VGA_COLOR_WHITE);
		kernel_writestring(str3);
	}
}


extern "C"
void kernel_main(void)
{
	/* Initialize terminal interface */
	kernel_initialize();

	shell_loop();
}
