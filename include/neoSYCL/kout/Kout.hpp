/********************************************************************
Copyright (c) 2021 Hiroyuki Takizawa

This software is released under the MIT License, see LICENSE.txt.
**********************************************************************/
#pragma once
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/PrettyPrinter.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Basic/Version.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/PreprocessorOptions.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include <regex>

#define SYCL_ACCESSOR "neosycl::sycl::accessor"
#define SYCL_HANDLER "class neosycl::sycl::handler"
#define SYCL_SINGLE_TASK "single_task"
#define SYCL_PARALLEL_FOR "parallel_for"

using namespace std;
using namespace llvm;
using namespace clang;
using namespace clang::tooling;
