/*
 * File: exception.h
 * Version: 1.0
 * Last modified on Sun Jul 24 10:26:20 1994 by eroberts
 * -----------------------------------------------------
 * The exception package provides a general exception
 * handling mechanism for use with C that is portable
 * across a variety of compilers and operating systems.
 */

#ifndef _exception_h
#define _exception_h

/*
 * Overview:
 * --------
 * The exception package makes it possible for clients to
 * specify a handler for an exceptional conditions in a
 * syntactically readable way.  As a client, your first step
 * is to declare an exception condition name by declaring
 * a variable of type exception, as in
 异常包使客户有可能以语法上可读的方式为一个异常条件指定一个处理程序。
 作为一个客户端，你的第一步是通过声明一个异常类型的变量来声明一个异常条件名称，如
 *
 *       exception MyException;
 *
 * Normal visibility rules apply, so that you should declare
 * the exception variable at the appropriate level.  For
 * example, if an exception is local to an implementation,
 * it should be declared statically within that module.  If
 * an exception condition is shared by many modules, the
 * exception variable should be declared in an interface
 * and exported to all clients that need it.  This package
 * defines and exports the exception ErrorException, which
 * is likely to be sufficient for many clients.
 * 
 *正常的可见性规则是适用的，所以你应该在适当的级别上声明“异常”变量。
 *例如，如果一个“异常”是一个实现的局部变量，那么它应该在该模块内静态地声明。
 *如果一个“异常”条件是由许多模块共享的，那么这个“异常”变量应该在一个接口中声明，并输出给所有需要它的客户端。
 *这个包定义并导出了异常ErrorException，这对许多客户端来说可能已经足够了。
 *
 * The basic functionality of exceptions is that one piece
 * of code can "raise" an exception so that it can then be
 * "handled" by special code in a dynamically enclosing
 * section of the program.  Exceptions are raised by calling
 * the pseudo-function raise with the exception name, as in
 * ”异常“的基本功能是，一段代码可以 "引发 "一个“异常”，
 *这样它就可以被程序中动态包围的部分的特殊代码所 "处理"。
 *“异常”是通过调用带有异常名称的伪函数 raise 来引发的，例如
 *
 *     raise(MyException);
 *
 * Exceptions are handled using the "try" statement
 * (actually implemented using macros), which has the form:
 * “异常”是用 "try "语句处理的 (实际上是用宏来实现的)，其形式为
 *
 *     try {
 *        . . . statements in the body of the block . . .
 *     except(exception1)
 *        . . . statements to handle exception 1 . . .
 *     except(exception2)
 *        . . . statements to handle exception 2 . . .
 *     except(ANY)
 *        . . . statements to handle any exception . . .
 *     } endtry
 *
 * Any number of except clauses may appear (up to a
 * maximum defined by the constant MaxExceptionsPerScope),
 * and the ANY clause is optional.
 *
 * When the program encounters the "try" statement, the
 * statements in the body are executed.  If no exception
 * conditions are raised during that execution, either
 * in this block or by a function call nested inside
 * this block, control passes to the end of the "try"
 * statement when the last statement in the block is
 * executed.  If an exception is raised during the
 * dynamic execution of the block, control immediately
 * passes to the statements in the appropriate except
 * clause.  Only the statements in that clause are
 * executed; no break statement is required to exit
 * the block.  If no handler for the raised exception
 * appears anywhere in the control history, the program
 * exits with an error.
 * 当程序遇到 "try "语句时，主体中的语句被执行。 
 如果在执行过程中，无论是在这个块中还是通过嵌套在这个块中的函数调用，
 都没有出现”异常“情况，那么当块中的最后一条语句被执行时，
 控制权就会转移到 "try "语句的结尾。 如果在该块的动态执行过程中出现了”异常“，
 控制权将立即转移到相应的 except 子句中的语句。 只有该子句中的语句被执行了；
 不需要break语句来退出该块。 如果在控制历史中没有出现“异常”处理程序，程序将以错误退出。
 *
 * Examples of use:
 *
 * 1.  Catching errors.
 *
 * The following code fragment traps calls to Error, so
 * that the program does not quit but instead returns
 * to the top-level read-and-execute loop.
 * 下面的代码片段捕获了对Error的调用，这样程序就不会退出，而是返回到顶层的读和执行的循环。
 *
 *     while (TRUE) {
 *         try {
 *             printf("> ");
 *             cmd = ReadCommand();
 *             ExecuteCommand(cmd);
 *         except(ErrorException)
 *             -- additional handling code, if any --
 *         } endtry
 *     }
 *
 * If either ReadCommand or ExecuteCommand calls Error,
 * control will be passed back to the main loop, after
 * executing any additional handler code.  (The error
 * message is printed by the Error routine, so it is
 * often the case that no additional work is needed.)
 *
 * 2.  Handling control-C
 *
 * The following code extends the example above so that
 * typing ^C also returns to top-level.
 *
 *     #include <signal.h>
 *
 *     static exception ControlCException;
 *     static int errorCount = 0;
 *     static int ControlCHandler();
 *
 *     main()
 *     {
 *         string cmd;
 *
 *         signal(SIGINT, ControlCHandler);
 *         while (TRUE) {
 *             try {
 *                 printf("> ");
 *                 cmd = ReadCommand();
 *                 ExecuteCommand(cmd);
 *             except(ControlCException);
 *                 printf("^C\n");
 *                 signal(SIGINT, ControlCHandler);
 *             except(ErrorException)
 *                 errorCount++;
 *             } endtry
 *         }
 *     }
 *
 *     static int ControlCHandler()
 *     {
 *         raise(ControlCException);
 *     }
 */

/*
 * Actual interface specification
 * ------------------------------
 * Most of the implementation of the exception mechanism is
 * actually done in the macros defined by this file.
 * Clients should ordinarily be able to read the description
 * above and ignore the detailed code below.
 */

#include <setjmp.h>
#include <string.h>
#include "genlib.h"

/* Define parameters and error status indicators */

#define MaxExceptionsPerScope 10
#define ETooManyExceptClauses 101
#define EUnhandledException 102

/* Codes to keep track of the state of the try handler */

#define ES_Initialize 0
#define ES_EvalBody 1
#define ES_Exception 2

/*
 * Type: exception
 * ---------------
 * Exceptions are specified by their address, so that the
 * actual structure does not matter.  Strings are used here
 * so that exporters of exceptions can store the exception
 * name for the use of debuggers and other tools.
 * 异常是由其地址指定的，所以实际的结构并不重要。 这里使用字符串是为了让异常的输出者可以存储异常的名称，供调试器和其他工具使用。
 */

typedef struct { string name; } exception;

/*
 * Type: context_block
 * -------------------
 * This structure is used internally to maintain a chain of
 * exception scopes on the control stack.
 */

typedef struct ctx_block {
    jmp_buf jmp;
    int nx;
    exception *array[MaxExceptionsPerScope];
    exception *id;
    void *value;
    string name;
    struct ctx_block *link;
} context_block;

/* Declare the built-in exceptions */

extern exception ErrorException;
extern exception ANY;

/* Declare a global pointer to the context stack */

extern context_block *exceptionStack;

/*
 * Function: RaiseException
 * Usage: RaiseException(&e, name, value);
 * ---------------------------------------
 * This function is called by the raise macro and does the
 * work necessary to raise the exception.  See the exception.c file
 * for details.  Clients do not ordinarily call this directly.
 */

void RaiseException(exception *e, string name, void *value);

/*
 * Function: HandlerExists
 * Usage: if (HandlerExists(&e)) ...
 * ---------------------------------
 * Determines whether a handler exists for an exception in
 * the dynamically enclosing scope.  Intended only for use
 * by special clients, such as the Error package.
 */

bool HandlerExists(exception *e);

/* Define the pseudo-functions for raise and try */

#define raise(e) RaiseException(&e, #e, NULL)

#define try \
      { \
          jmp_buf _tmpbuf; \
          context_block _ctx; \
          volatile int _es; \
          _es = ES_Initialize; \
          _ctx.nx = 0; \
          _ctx.link = exceptionStack; \
          exceptionStack = (context_block *) &_ctx; \
          if (setjmp(_tmpbuf) != 0) _es = ES_Exception; \
          memcpy((void *) _ctx.jmp, (void *) _tmpbuf, sizeof(jmp_buf)); \
          while (1) { \
              if (_es == ES_EvalBody)

#define except(e) \
                  if (_es == ES_EvalBody) exceptionStack = _ctx.link; \
                  break; \
              } \
              if (_es == ES_Initialize) { \
                  if (_ctx.nx >= MaxExceptionsPerScope) \
                      exit(ETooManyExceptClauses); \
                  _ctx.array[_ctx.nx++] = &e; \
              } else if (_ctx.id == &e || &e == &ANY) { \
                  exceptionStack = _ctx.link;

#define endtry \
              if (_es != ES_Initialize) break; \
              _es = ES_EvalBody; \
          } \
      }

#define GetExceptionName() _ctx.name
#define GetExceptionValue() _ctx.value
#define GetCurrentException() _ctx.id

#endif
