#ifdef WIN32
#include <windows.h>
#else /* same for most platforms */
#include <termios.h>
#include <unistd.h>
#endif

void setecho(int state) {
#ifdef WIN32
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    if(!state) mode &= ~ENABLE_ECHO_INPUT;
    else mode |= ENABLE_ECHO_INPUT;
    SetConsoleMode(hStdin, mode );
#else /* same for most platforms */
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    if(!state) tty.c_lflag &= ~(0u | ECHO);
    else tty.c_lflag |= (0u | ECHO);
    (void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}
