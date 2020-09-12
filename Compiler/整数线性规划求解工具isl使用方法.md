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
        
* 2.返回的结果

isl isl_size对返回非负值（通常是数字或位置）的函数使用特殊的返回类型。除了常规的非负返回值外，isl_size_error还可能返回一个特殊的（负）值，表明出错。

isl对于返回布尔值或原则上不返回任何值的函数，它也使用两种特殊的返回类型。特别是，该isl_bool类型具有三个可能的值：（isl_bool_true一个正整数），表示true或yes；isl_bool_false（整数值零），表示false或no；和isl_bool_error（负整数值），表示出了点问题。在上定义了以下操作isl_bool。该函数isl_bool_not可以被用于否定的isl_bool，其中的否定isl_bool_error是isl_bool_error一次。该函数isl_bool_ok将整数转换为isl_bool。任何非零值的收益率isl_bool_true和零收益率isl_bool_false。

```c
bool
int a = 0;
isl_bool result = isl_bool_ok(a);
cout << result << endl;
 ```
输出结果为0


* 3.isl_val对象

一个 isl_val 对象表示整数值，有理值或三个特殊值之一，即无穷大，负无穷大和正无穷大。可以使用以下功能创建一些预定义的值：
```c
#include <isl/val.h>
__isl_give isl_val *isl_val_zero(isl_ctx *ctx);
__isl_give isl_val *isl_val_one(isl_ctx *ctx);
__isl_give isl_val *isl_val_negone(isl_ctx *ctx);
__isl_give isl_val *isl_val_nan(isl_ctx *ctx);
__isl_give isl_val *isl_val_infty(isl_ctx *ctx);
__isl_give isl_val *isl_val_neginfty(isl_ctx *ctx);
```
测试：
```c
isl_val * val = isl_val_zero(isl);
cout << val << endl;
```
输出：

0x93c6080

可以使用以下函数创建特定的整数值：
```c
#include <isl/val.h>
__isl_give isl_val *isl_val_int_from_si(isl_ctx *ctx,
        long i);
__isl_give isl_val *isl_val_int_from_ui(isl_ctx *ctx,
        unsigned long u);
__isl_give isl_val *isl_val_int_from_chunks(isl_ctx *ctx,
        size_t n, size_t size, const void *chunks);
```
一个isl_val可以使用下面的函数来修改：
```c
#include <isl/val.h>
__isl_give isl_val *isl_val_set_si(__isl_take isl_val *v,
        long i);
```
可以使用以下功能复制和释放 isl_val 。
```c
#include <isl/val.h>
__isl_give isl_val *isl_val_copy(__isl_keep isl_val *v);
__isl_null isl_val *isl_val_free(__isl_take isl_val *v);
```
对于这样的数值对象我们还可以对其进行一元运算、二元运算等，具体运算函数可以参考文档。


* 3.集合（set）与关系或映射（map）

isl使用六种类型的代表集合和关系的对象，

isl_basic_set，

isl_basic_map，

isl_set，

isl_map，

isl_union_set  和

isl_union_map（这些其实都是对象）。

isl_basic_set和isl_basic_map分别代表可仿射约束的集合和关系，而isl_set和isl_map分别代表isl_basic_sets和isl_basic_maps 的并集。但是，所有isl_basic_sets或isl_basic_maps都必须居住在同一空间中。isl_union_sets和isl_union_maps表示isl_sets或isl_maps在不同空间中的并集，如果空间的维数和/或名称不同，则它们被认为是不同的（请参见“空间”）。集和关系（映射）之间的区别在于，set具有一组变量，而map具有两组变量，即输入变量和输出变量。

* 4.错误处理

isl在触发运行时错误时，支持不同的反应方式。例如，如果isl_map_intersect用两个具有不兼容空间的映射调用诸如之类的函数，则会出现运行时错误。有三种对错误做出反应的方式：警告，继续或中止。

默认行为是警告。在这种模式下，isl打印警告，将最后一个错误存储在相应isl_ctx的错误中，触发错误的函数将返回一个值，指示发生了某些错误。如果函数返回指针，则该值为NULL。在返回一个功能的情况下isl_size，isl_bool或者isl_stat，这个值isl_size_error，isl_bool_error或isl_stat_error。错误不会破坏内部状态，因此可以继续使用isl。isl还提供了读取上一个错误的功能，包括特定的错误消息，发生错误的isl源文件和行号，以及重置有关上一个错误的所有信息。最后的错误仅用于提供信息。它的存在不会改变的行为isl。因此，重新设置错误并不需要继续使用isl，而只是为了观察新的错误。



* 5.isl_id对象

标识符用于识别单个维度和维度元组。它们由一个可选的名称和一个可选的用户指针（代表该id对象标识哪个对象）组成。但是，名称和用户指针不能都为NULL。具有相同名称但指针值不同的标识符被认为是不同的。同样，具有不同名称但指针值相同的标识符也被认为是不同的。相同的标识符使用相同的对象表示。因此，可以使用==运算符测试成对的标识符是否相等。可以使用以下功能构造，复制，释放，检查和打印标识符。


```c
#include <isl/id.h>
__isl_give isl_id *isl_id_alloc(isl_ctx *ctx,
        __isl_keep const char *name, void *user);
__isl_give isl_id *isl_id_set_free_user(
        __isl_take isl_id *id,
        void (*free_user)(void *user));
__isl_give isl_id *isl_id_copy(isl_id *id);
__isl_null isl_id *isl_id_free(__isl_take isl_id *id);

void *isl_id_get_user(__isl_keep isl_id *id);
__isl_keep const char *isl_id_get_name(__isl_keep isl_id *id);

__isl_give isl_printer *isl_printer_print_id(
        __isl_take isl_printer *p, __isl_keep isl_id *id);
```


使用示例：
```c
isl_set *sum = isl_set_sum(set, set2); // 表示对set、set2这两个集合对象进行加法
isl_id *ids = isl_id_alloc(ctx, "sum", sum);// 定义一个id指向sum
char *str_id = isl_id_to_str(ids);
printf("id:%s\n", str_id);
```
最后输出如下，代表一个地址值：

id:sum@0x993a9e0



* 6.isl_space对象

每当从头开始创建新的set，map或类似对象时，都需要使用来指定其所处的空间isl_space。

每个空间包含零个或多个参数，以及零个，一个或两个元组的 set 或输入/输出维。参数和尺寸由isl_dim_type和标识。类型isl_dim_param是指参数，类型isl_dim_set是指设置维（对于具有单个元组的空间）和类型isl_dim_in，而类型isl_dim_out是指输入和输出维（对于具有两个元组的空间）。局部空间也包含类型的尺寸isl_dim_div。注意，参数仅由其在给定对象中的位置来标识。在不同的对象之间，参数（通常）由其名称或标识符标识。只有未命名的参数才能通过它们在对象之间的位置来标识。不建议使用未命名的参数。

对space的基本操作如下：
```c
#include <isl/space.h>
__isl_give isl_space *isl_space_unit(isl_ctx *ctx);
__isl_give isl_space *isl_space_alloc(isl_ctx *ctx,
        unsigned nparam, unsigned n_in, unsigned n_out);//带进带出的专用于map
__isl_give isl_space *isl_space_params_alloc(isl_ctx *ctx,
        unsigned nparam);
__isl_give isl_space *isl_space_set_alloc(isl_ctx *ctx,
        unsigned nparam, unsigned dim);
__isl_give isl_space *isl_space_copy(__isl_keep isl_space *space);
__isl_null isl_space *isl_space_free(__isl_take isl_space *space);

```

需要使用isl_space_unit或创建用于创建参数域的空间isl_space_params_alloc。对于其他集合，需要使用来创建空间isl_space_set_alloc，而对于关系而言，需要使用来创建空间isl_space_alloc。使用的isl_space_params_alloc，isl_space_set_alloc并且isl_space_alloc因为它们允许引进未命名参数是不鼓励。

要检查给定空间是集合空间还是映射空间，还是参数空间，请使用以下函数：
```c
#include <isl/space.h>
isl_bool isl_space_is_params(__isl_keep isl_space *space);
isl_bool isl_space_is_set(__isl_keep isl_space *space);
isl_bool isl_space_is_map(__isl_keep isl_space *space);
```


创建与其他对象位于同一空间的对象通常很有用。这可以通过基于原始对象的空间来创建新对象（请参见“创建新的集合和关系”或“函数”）来完成。

创建和一个多面体对象相同space的space：
```c
#include <isl/polynomial.h>
isl_space *isl_qpolynomial_get_domain_space(isl_qpolynomial *qp);
isl_space *isl_qpolynomial_get_space(isl_qpolynomial *qp);
isl_space *isl_qpolynomial_fold_get_domain_space(
        __isl_keep isl_qpolynomial_fold *fold);
__isl_give isl_space *isl_qpolynomial_fold_get_space(
        __isl_keep isl_qpolynomial_fold *fold);
__isl_give isl_space *isl_pw_qpolynomial_get_domain_space(
        __isl_keep isl_pw_qpolynomial *pwqp);
__isl_give isl_space *isl_pw_qpolynomial_get_space(
        __isl_keep isl_pw_qpolynomial *pwqp);
__isl_give isl_space *isl_pw_qpolynomial_fold_get_domain_space(
        __isl_keep isl_pw_qpolynomial_fold *pwf);
__isl_give isl_space *isl_pw_qpolynomial_fold_get_space(
        __isl_keep isl_pw_qpolynomial_fold *pwf);
__isl_give isl_space *isl_union_pw_qpolynomial_get_space(
        __isl_keep isl_union_pw_qpolynomial *upwqp);
__isl_give isl_space *isl_union_pw_qpolynomial_fold_get_space(
        __isl_keep isl_union_pw_qpolynomial_fold *upwf);
```

可以使用以下功能将其他参数添加到空间。
```c
#include <isl/space.h>
__isl_give isl_space *isl_space_add_param_id(
        __isl_take isl_space *space,
        __isl_take isl_id *id);
```
相反，可以使用以下函数从空格中删除所有参数。
```c
#include <isl/space.h>
__isl_give isl_space *isl_space_drop_all_params(
        __isl_take isl_space *space);
```

## isl基本操作


* 1.create set & map

  * Empty sets and relations

可以看出来。set和map的创建时凭借于space的。

```c
__isl_give isl_basic_set *isl_basic_set_empty(
        __isl_take isl_space *space);//创建一个空的set空间
__isl_give isl_basic_map *isl_basic_map_empty(
        __isl_take isl_space *space);
__isl_give isl_set *isl_set_empty(
        __isl_take isl_space *space);
__isl_give isl_map *isl_map_empty(
        __isl_take isl_space *space);
__isl_give isl_union_set *isl_union_set_empty(
        __isl_take isl_space *space);
__isl_give isl_union_map *isl_union_map_empty(
        __isl_take isl_space *space);

```


  * Universe sets and relations

```c
_isl_give isl_basic_set *isl_basic_set_universe(
        __isl_take isl_space *space);
__isl_give isl_basic_map *isl_basic_map_universe(
        __isl_take isl_space *space);
__isl_give isl_set *isl_set_universe(
        __isl_take isl_space *space);
__isl_give isl_map *isl_map_universe(
        __isl_take isl_space *space);
__isl_give isl_union_set *isl_union_set_universe(
        __isl_take isl_union_set *uset);
__isl_give isl_union_map *isl_union_map_universe(
        __isl_take isl_union_map *umap);
```

