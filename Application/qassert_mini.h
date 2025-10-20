/******************************************************************************
* @file    qassert_mini.h
* @brief   Minimal design-by-contract (assert) interface for embedded projects
* @author  Alexandre Panhaleux
* @license MIT
******************************************************************************/

#ifndef QASSERT_MINI_H_
#define QASSERT_MINI_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef Q_NORETURN
#define Q_NORETURN _Noreturn void
#endif

/* App must provide this (you do in bsp.c) */
Q_NORETURN Q_onAssert(const char *module, int location);

/* Tag each module for nicer assert messages */
#define Q_DEFINE_THIS_MODULE(name_) \
    static const char Q_this_module_[] = (name_);

/* Fallback: use file name if you prefer not to name modules explicitly */
#define Q_DEFINE_THIS_FILE Q_DEFINE_THIS_MODULE(__FILE__)

/* Core assertions (line-based and ID-based) */
#define Q_ASSERT(expr_)          ((expr_) ? (void)0 : Q_onAssert(Q_this_module_, __LINE__))
#define Q_ASSERT_ID(id_, expr_)  ((expr_) ? (void)0 : Q_onAssert(Q_this_module_, (id_)))
#define Q_ERROR()                Q_onAssert(Q_this_module_, __LINE__)
#define Q_ERROR_ID(id_)          Q_onAssert(Q_this_module_, (id_))

/* Always evaluate, never call handler (useful in release) */
#define Q_ALLEGE(expr_)          ((void)(expr_))

/* Lightweight compile-time assert (keeps C99/C11 compatibility) */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
  #define Q_ASSERT_STATIC(expr_) _Static_assert((expr_), "Q_ASSERT_STATIC failed")
#else
  #define Q_ASSERT_STATIC(expr_) extern char q_static_assert[(expr_) ? 1 : -1]
#endif

#ifdef __cplusplus
}
#endif
#endif /* QASSERT_MINI_H_ */
