/***************************************************************
 * Copyright 2020 (c) Xinguang Bian <victorbian@outlook.com>
 *
 * WarningMessage is part of TestFunc project.
 *
 * Note: C++17 feature is required.
 **************************************************************/

#ifndef __DBG_WARNING_MESSAGE_H
#define __DBG_WARNING_MESSAGE_H

#include <iostream>

#define TESTFUNCS_CREATE_WANNING_MSG(name)                                                                                  \
    namespace details {                                                                                           \
    class name##Dummy final {                                                                                     \
        struct Msg {                                                                                              \
            Msg()                                                                                                 \
            {                                                                                                     \
                std::cout << "\33[33mWarning: Debug feature \"" << #name << "\" is included.\33[0m" << std::endl; \
            };                                                                                                    \
        };                                                                                                        \
        inline static Msg msg;                                                                                    \
    };                                                                                                            \
    } // namespace details

#endif
