# 整数线性规划求解工具 Integer Set Library
[代码](http://isl.gforge.inria.fr/)

[参考](https://blog.csdn.net/wangbowj123/article/details/101854176)

isl是一个用于多面体模型调度实现的c/c++库。通过isl，我们可以对模型进行自动的调度，循环优化等。

## 编译

    ISL,Integer Set Library
    版本0.22.1，http://isl.gforge.inria.fr/
    README关于编译的部分说了
    ./configure
    make
    make install
    使用./configure --help阅读后针对本地环境，使用
    ./configure --prefix=/opt/isl --enable-shared=yes --enable-static=yes \
    CC=/opt/gcc/bin/gcc \
    CFLAGS="-O3 -I/opt/gmp/include" \
    LDFLAGS="-L/opt/gmp/lib -Wl,--rpath=/opt/gmp/lib" \
    CXX=/opt/gcc-10.2.0/bin/g++ \
    CXXFLAGS="-I/opt/gmp/include" \
    PYTHON=/opt/python3/bin/python3 \
    --enable-fast-install=no --with-gnu-ld
    make
    make install

[islpy python接口](https://github.com/inducer/islpy)

 
## isl内部对象介绍

* 1.isl_ctx对象的创建（初始化）：

整数集和关系的所有操作都在的上下文中发生isl_ctx。给定isl_ctx只能在单个线程中使用。一个函数的所有参数都必须在同一上下文中分配。当前没有功能可用于将对象从一个移动isl_ctx到另一个isl_ctx。这意味着当前无法安全地将对象从一个线程移动到另一个线程，除非整个线程isl_ctx都移动了。

一个isl_ctx可以使用分配isl_ctx_alloc和使用中解脱出来isl_ctx_free。在isl_ctx释放之前，应释放内分配的所有对象isl_ctx。

isl_ctx 对象就像一根线一样把上下文关系（对象）连接起来。
```c
isl_ctx *isl_ctx_alloc();
void isl_ctx_free(isl_ctx *ctx);
```
测试：
```c
isl_ctx *isl = isl_ctx_alloc(); // 创建对象
cout << isl << endl;
isl_ctx_free(isl); // 释放
```
输出是一个地址值，可以看出来该指针指向一个对象。

同时，我们还可以为 isl_ctx 对象指定低级 op 数量的范围（bound）。例如：
```c
void isl_ctx_set_max_operations(isl_ctx *ctx,
        unsigned long max_operations);
unsigned long isl_ctx_get_max_operations(isl_ctx *ctx);
void isl_ctx_reset_operations(isl_ctx *ctx);
```
上面说了ctx相当于线将各种对象连接起来，那么我们还可以通过上下文对象来得到ctx：
```c
#include <isl/val.h>
isl_ctx *isl_val_get_ctx(__isl_keep isl_val *val);
isl_ctx *isl_multi_val_get_ctx(
        __isl_keep isl_multi_val *mv);

#include <isl/id.h>
isl_ctx *isl_id_get_ctx(__isl_keep isl_id *id);
isl_ctx *isl_multi_id_get_ctx(
        __isl_keep isl_multi_id *mi);

#include <isl/local_space.h>
isl_ctx *isl_local_space_get_ctx(
        __isl_keep isl_local_space *ls);

#include <isl/set.h>
isl_ctx *isl_set_list_get_ctx(
        __isl_keep isl_set_list *list);

#include <isl/aff.h>
isl_ctx *isl_aff_get_ctx(__isl_keep isl_aff *aff);
isl_ctx *isl_multi_aff_get_ctx(
        __isl_keep isl_multi_aff *maff);
isl_ctx *isl_pw_aff_get_ctx(__isl_keep isl_pw_aff *pa);
isl_ctx *isl_pw_multi_aff_get_ctx(
        __isl_keep isl_pw_multi_aff *pma);
isl_ctx *isl_multi_pw_aff_get_ctx(
        __isl_keep isl_multi_pw_aff *mpa);
isl_ctx *isl_union_pw_aff_get_ctx(
        __isl_keep isl_union_pw_aff *upa);
isl_ctx *isl_union_pw_multi_aff_get_ctx(
        __isl_keep isl_union_pw_multi_aff *upma);
isl_ctx *isl_multi_union_pw_aff_get_ctx(
        __isl_keep isl_multi_union_pw_aff *mupa);

#include <isl/id_to_ast_expr.h>
isl_ctx *isl_id_to_ast_expr_get_ctx(
        __isl_keep isl_id_to_ast_expr *id2expr);

#include <isl/point.h>
isl_ctx *isl_point_get_ctx(__isl_keep isl_point *pnt);

#include <isl/vec.h>
isl_ctx *isl_vec_get_ctx(__isl_keep isl_vec *vec);

#include <isl/mat.h>
isl_ctx *isl_mat_get_ctx(__isl_keep isl_mat *mat);

#include <isl/vertices.h>
isl_ctx *isl_vertices_get_ctx(
        __isl_keep isl_vertices *vertices);
isl_ctx *isl_vertex_get_ctx(__isl_keep isl_vertex *vertex);
isl_ctx *isl_cell_get_ctx(__isl_keep isl_cell *cell);

#include <isl/flow.h>
isl_ctx *isl_restriction_get_ctx(
        __isl_keep isl_restriction *restr);
isl_ctx *isl_union_access_info_get_ctx(
        __isl_keep isl_union_access_info *access);
isl_ctx *isl_union_flow_get_ctx(
        __isl_keep isl_union_flow *flow);

#include <isl/schedule.h>
isl_ctx *isl_schedule_get_ctx(
        __isl_keep isl_schedule *sched);
isl_ctx *isl_schedule_constraints_get_ctx(
        __isl_keep isl_schedule_constraints *sc);

#include <isl/schedule_node.h>
isl_ctx *isl_schedule_node_get_ctx(
        __isl_keep isl_schedule_node *node);

#include <isl/ast_build.h>
isl_ctx *isl_ast_build_get_ctx(
        __isl_keep isl_ast_build *build);

#include <isl/ast.h>
isl_ctx *isl_ast_expr_get_ctx(
        __isl_keep isl_ast_expr *expr);
isl_ctx *isl_ast_node_get_ctx(
        __isl_keep isl_ast_node *node);

#include <isl/stride_info.h>
isl_ctx *isl_stride_info_get_ctx(
        __isl_keep isl_stride_info *si);

#include <isl/fixed_box.h>
isl_ctx *isl_fixed_box_get_ctx(
        __isl_keep isl_fixed_box *box);
```
        
