
M. Eymen Ünay\

*Istanbul Technical University - BLG 335E Analysis of Algorithms I - Assignment II*

*This assignment received full grade.*

# Introduction

The assignment's requirement is to process a stream of numbers and
evaluate statistics. As the assignment states it, an incremental
approach to evaluation is embraced rather than analysing the whole input
file beforehand. As every statistical feature can be deactivated at the
beginning of the input file, modularity at evaluation of each
statistical value is preserved. This modularity caused several similar
data structures and similar calls to these structures but the ease of
maintenance and deactivation ability made me favour a more modular
approach.

# Code Internals

## Data Structures

To store which statistical values are desired at output a struct with
every value corresponding to a bool is created and checked throughout
the code to prevent unnecessary calculations or writes.

Every statistical value and their companions are stored in their
corresponding struct in a global data_structures struct. Additional to
their own values, the structs contain companion structures. Std has all
the relevant data in a vector which it computes with. Each of median,
firstq and thirdq has two heaps, a max and min heap. Quartiles have 3:1
size ratios of heaps and median has 1:1 size ratio.

## Functions

To optimize for performance, costly operations such as sorting is
avoided and instead heaps are used to take the load of storing minimum
and maximums of regions of interest. Functions about heap are in heap.h.

The worst case recurrence relation of a Max_heapify function with
pseudocode at Algorithm 1, is $T(n) <= T(2n/3) + \Theta(1)$. Using
Case 2 of Master Method $T(n) = O(lg n)$

```pseudocode
Algorithm 1: Max heapify

Maxheap M

function Max_heapify(M, i):
    l ← Left(i)
    r ← Right(i)
    largest ← i
    
    # if left child is larger make it the largest
    if (l ≤ heap_size and A[l] > A[i]):
        largest ← l
    else:
        largest ← i
    
    # if right child is larger make it the largest
    if (r ≤ heap_size and A[r] > A[largest]):
        largest ← r
    
    # if one of the childs were larger
    if largest ≠ i:
        Swap(A[i], A[largest])  # swap largest child with index at parent
        Max_heapify(A, largest)  # recursive call to itself from child

```

The Algorithm 2 provides the ability to build a heap
from an unsorted array though it is not used as every heap is built from
scratch. Its asymptotic bound is $O(n)$.

```pseudocode
Algorithm 2: Build_Max_Heap

Array A

function Build_Max_Heap(A):
    heap_size ← Length(A)
    
    # Runs from the parents with children
    for i ← Floor(Length(A)/2) downto 1:
        Max_heapify(A, i)
```

The Algorithm 3 returns the root node and deletes it
at the heap. Calls Algorithm 1 to restore the heap property. Its analysis
is similar to Algorithm 1 since the remaining code adds constant
complexity.

```pseudocode
Algorithm 3: Extract_Max_Heap

Heap M

function Extract_Max_Heap(A):
    max ← M[1]  # Root node
    M[1] ← M[heap_size]  # last element becomes the root
    heap_size--
    Pop_back(M)  # last element deleted
    Max_Heapify(M, 1)
    return max
```

# Application of Data Structures

There are two classes which coordinate and operate on the required
inputs: Stats class which has the calculate functions and Manager class
which coordinates when to call calculate and adds new data to the
corresponding data structures.

When new data arrives add_element function of Manager runs and checks
which options are set. Accordingly, runs calculate functions of mean
std, renews min and max, adds data to std's vector and adds elements to
heaps of order statistical values. After adding data to vector and heaps
their corresponding statistical calculation functions at Stats are
called.

Double heap structures for order statistics is used. To implement
median, two cases are checked. If sizes of heaps are equal, new element
is added to max heap and the heap gets sifted which updates the min
heap. The case for different size is similar with the direction in
reverse. To calculate median sizes are checked and returned from max or
interpolated accordingly.

Achieving similar structure at quartiles requires 4 states to check but
is similar to median in essence. The calculate function uses Gumbel
quartile interpolation according to new states after the element is
added.

# Performance Analysis

To minimise the effects of standard deviation and mean calculations,
Welford's online algorithm is used. Compared to naive implementation, it
reduced the growth complexity close to $O(logn)$ as it can be estimated
from the plot below referring heap based order calculation complexities.

![](./res/exec_plot.png)

```mermaid
graph TD
node_1[frame_dummy 10000percent 10000percent 9x]
node_8[stdvectoroperator 10000percent 10000percent 201152x]
node_9[stdvectorsize 10000percent 10000percent 101960x]
node_10[stdvectoroperator 10000percent 10000percent 43266x]
node_11[stdremove_referencetype stdmove 10000percent 10000percent 8442x]
node_12[__gnu_cxx__enable_if__type stdfloor 10000percent 10000percent 4469x]
node_13[float const stdforward 10000percent 10000percent 4280x]
node_14[bool stdoperator 10000percent 10000percent 3357x]
node_15[stdenable_iftype stdswap 10000percent 10000percent 2812x]
node_16[maxheapget_size 10000percent 10000percent 2688x]
node_17[minheapget_size 10000percent 10000percent 2688x]
node_18[operator new 10000percent 10000percent 2570x]
node_19[minheapparent 10000percent 10000percent 2271x]
node_20[minheapleft 10000percent 10000percent 2245x]
node_21[minheapright 10000percent 10000percent 2245x]
node_22[maxheapparent 10000percent 10000percent 2198x]
node_23[void __gnu_cxxnew_allocatorconstruct 10000percent 10000percent 2116x]
node_24[void stdallocator_traitsconstruct 10000percent 10000percent 2116x]
node_25[stdvectorpush_back 10000percent 10000percent 2116x]
node_26[maxheapget_max 10000percent 10000percent 1680x]
node_27[minheapget_min 10000percent 10000percent 1456x]
node_28[maxheapleft 10000percent 10000percent 1339x]
node_29[maxheapright 10000percent 10000percent 1339x]
node_30[minheapinsert 10000percent 10000percent 1089x]
node_31[maxheapinsert 10000percent 10000percent 1027x]
node_32[stdvectorsize 10000percent 10000percent 988x]
node_33[double const stdforward 10000percent 10000percent 906x]
node_34[void __gnu_cxxnew_allocatordestroy 10000percent 10000percent 772x]
node_35[void stdallocator_traitsdestroy 10000percent 10000percent 772x]
node_36[stdvectorpop_back 10000percent 10000percent 772x]
node_37[Statscalculate_std 10000percent 10000percent 448x]
node_38[Statscalculate_mean 10000percent 10000percent 448x]
node_39[Statscalculate_firstq 10000percent 10000percent 448x]
node_40[Statscalculate_median 10000percent 10000percent 448x]
node_41[Statscalculate_thirdq 10000percent 10000percent 448x]
node_42[manageradd_element 10000percent 10000percent 448x]
node_43[manageradd_element_heap_firstq 10000percent 10000percent 448x]
node_44[manageradd_element_heap_median 10000percent 10000percent 448x]
node_45[manageradd_element_heap_thirdq 10000percent 10000percent 448x]
node_46[void __gnu_cxxnew_allocatorconstruct 10000percent 10000percent 448x]
node_47[float __gnu_cxx__stoa 10000percent 10000percent 448x]
node_48[void stdallocator_traitsconstruct 10000percent 10000percent 448x]
node_49[stdvectorpush_back 10000percent 10000percent 448x]
node_50[std__cxx11stof 10000percent 10000percent 448x]
node_51[__gnu_cxx__stoa_Range_chk_S_chk 10000percent 10000percent 448x]
node_52[__gnu_cxx__stoa_Save_errno_Save_errno 10000percent 10000percent 448x]
node_53[__gnu_cxx__stoa_Save_errno_Save_errno 10000percent 10000percent 448x]
node_54[minheapextract_min 10000percent 10000percent 417x]
node_55[minheapmin_heapify 10000percent 10000percent 2245x]
node_56[maxheapextract_max 10000percent 10000percent 355x]
node_57[maxheapmax_heapify 10000percent 10000percent 1339x]
node_58[float std__niter_base 10000percent 10000percent 324x]
node_59[Statsprint 10000percent 10000percent 320x]
node_60[Statswrite 10000percent 10000percent 320x]
node_61[__gnu_cxx__normal_iteratorbase 10000percent 10000percent 216x]
node_62[__gnu_cxxnew_allocatormax_size 10000percent 10000percent 162x]
node_63[unsigned long const stdmin 10000percent 10000percent 128x]
node_64[std_Vector_base_M_get_Tp_allocator 10000percent 10000percent 114x]
node_65[__gnu_cxx__normal_iterator__normal_iterator 10000percent 10000percent 108x]
node_66[std_Vector_base_M_get_Tp_allocator 10000percent 10000percent 108x]
node_67[stdvectormax_size 10000percent 10000percent 108x]
node_68[stdallocator_traitsmax_size 10000percent 10000percent 108x]
node_69[stdvector_S_max_size 10000percent 10000percent 108x]
node_70[stdvector_S_relocate 10000percent 10000percent 108x]
node_71[stdvector_S_do_relocate 10000percent 10000percent 108x]
node_72[stdvector_S_use_relocate 10000percent 10000percent 108x]
node_73[stdvector_S_nothrow_relocate 10000percent 10000percent 108x]
node_74[float std__relocate_a 10000percent 10000percent 108x]
node_75[stdenable_iftype std__relocate_a_1 10000percent 10000percent 108x]
node_76[unsigned long const stdmax 10000percent 10000percent 64x]
node_77[std_Vector_base_M_deallocate 10000percent 10000percent 60x]
node_78[double std__niter_base 10000percent 10000percent 60x]
node_79[__gnu_cxxnew_allocatordeallocate 10000percent 10000percent 54x]
node_80[__gnu_cxxnew_allocatorallocate 10000percent 10000percent 54x]
node_81[__gnu_cxx__normal_iteratordifference_type __gnu_cxxoperator- 10000percent 10000percent 54x]
node_82[stdvector_M_check_len 10000percent 10000percent 54x]
node_83[std_Vector_base_M_allocate 10000percent 10000percent 54x]
node_84[stdallocator_traitsdeallocate 10000percent 10000percent 54x]
node_85[stdallocator_traitsallocate 10000percent 10000percent 54x]
node_86[stdvectorend 10000percent 10000percent 54x]
node_87[stdvectorbegin 10000percent 10000percent 54x]
node_88[void stdvector_M_realloc_insert 10000percent 10000percent 48x]
node_89[__gnu_cxx__normal_iteratorbase 10000percent 10000percent 40x]
node_90[__gnu_cxxnew_allocatormax_size 10000percent 10000percent 30x]
node_91[float stdforward 10000percent 10000percent 24x]
node_92[std_Vector_base_M_get_Tp_allocator 10000percent 10000percent 21x]
node_93[__gnu_cxx__normal_iterator__normal_iterator 10000percent 10000percent 20x]
node_94[std_Vector_base_M_get_Tp_allocator 10000percent 10000percent 20x]
node_95[stdvectormax_size 10000percent 10000percent 20x]
node_96[stdallocator_traitsmax_size 10000percent 10000percent 20x]
node_97[stdvector_S_max_size 10000percent 10000percent 20x]
node_98[stdvector_S_relocate 10000percent 10000percent 20x]
node_99[stdvector_S_do_relocate 10000percent 10000percent 20x]
node_100[stdvector_S_use_relocate 10000percent 10000percent 20x]
node_101[stdvector_S_nothrow_relocate 10000percent 10000percent 20x]
node_102[double std__relocate_a 10000percent 10000percent 20x]
node_103[stdenable_iftype std__relocate_a_1 10000percent 10000percent 20x]
node_104[std_Vector_base_M_deallocate 10000percent 10000percent 11x]
node_105[__gnu_cxxnew_allocatordeallocate 10000percent 10000percent 10x]
node_106[__gnu_cxxnew_allocatorallocate 10000percent 10000percent 10x]
node_107[__gnu_cxx__normal_iteratordifference_type __gnu_cxxoperator- 10000percent 10000percent 10x]
node_108[stdvector_M_check_len 10000percent 10000percent 10x]
node_109[std_Vector_base_M_allocate 10000percent 10000percent 10x]
node_110[stdallocator_traitsdeallocate 10000percent 10000percent 10x]
node_111[stdallocator_traitsallocate 10000percent 10000percent 10x]
node_112[void stdvector_M_realloc_insert 10000percent 10000percent 10x]
node_113[stdvectorend 10000percent 10000percent 10x]
node_114[stdvectorbegin 10000percent 10000percent 10x]
node_115[void __gnu_cxxnew_allocatorconstruct 10000percent 10000percent 6x]
node_116[__gnu_cxxnew_allocatornew_allocator 10000percent 10000percent 6x]
node_117[__gnu_cxxnew_allocatornew_allocator 10000percent 10000percent 6x]
node_118[stdallocatorallocator 10000percent 10000percent 6x]
node_119[stdallocatorallocator 10000percent 10000percent 6x]
node_120[void std_Destroy_aux__destroy 10000percent 10000percent 6x]
node_121[std_Vector_base_Vector_impl_Vector_impl 10000percent 10000percent 6x]
node_122[std_Vector_base_Vector_impl_Vector_impl 10000percent 10000percent 6x]
node_123[std_Vector_base_Vector_impl_data_Vector_impl_data 10000percent 10000percent 6x]
node_124[std_Vector_base_Vector_base 10000percent 10000percent 6x]
node_125[std_Vector_base_Vector_base 10000percent 10000percent 6x]
node_126[void stdallocator_traitsconstruct 10000percent 10000percent 6x]
node_127[void stdvectoremplace_back 10000percent 10000percent 6x]
node_128[void stdvector_M_realloc_insert 10000percent 10000percent 6x]
node_129[stdvectorpush_back 10000percent 10000percent 6x]
node_130[stdvectorvector 10000percent 10000percent 6x]
node_131[stdvectorvector 10000percent 10000percent 6x]
node_132[void std_Destroy 10000percent 10000percent 6x]
node_133[void std_Destroy 10000percent 10000percent 6x]
node_134[maxheapmaxheap 10000percent 10000percent 3x]
node_135[maxheapmaxheap 10000percent 10000percent 3x]
node_136[minheapminheap 10000percent 10000percent 3x]
node_137[minheapminheap 10000percent 10000percent 3x]
node_138[int __gnu_cxx__stoa 10000percent 10000percent 2x]
node_139[std__cxx11stoi 10000percent 10000percent 2x]
node_140[__gnu_cxx__stoa_Range_chk_S_chk 10000percent 10000percent 2x]
node_141[__gnu_cxx__stoa_Save_errno_Save_errno 10000percent 10000percent 2x]
node_142[__gnu_cxx__stoa_Save_errno_Save_errno 10000percent 10000percent 2x]
node_143[__static_initialization_and_destruction_0 10000percent 10000percent 1x]
node_144[reader 10000percent 10000percent 1x]
node_145[StatsStats 10000percent 10000percent 1x]
node_146[__gnu_cxxnew_allocatornew_allocator 10000percent 10000percent 1x]
node_147[__gnu_cxxnew_allocatornew_allocator 10000percent 10000percent 1x]
node_148[stdallocatorallocator 10000percent 10000percent 1x]
node_149[stdallocatorallocator 10000percent 10000percent 1x]
node_150[void std_Destroy_aux__destroy 10000percent 10000percent 1x]
node_151[std_Vector_base_Vector_impl_Vector_impl 10000percent 10000percent 1x]
node_152[std_Vector_base_Vector_impl_Vector_impl 10000percent 10000percent 1x]
node_153[std_Vector_base_Vector_impl_data_Vector_impl_data 10000percent 10000percent 1x]
node_154[std_Vector_base_Vector_base 10000percent 10000percent 1x]
node_155[std_Vector_base_Vector_base 10000percent 10000percent 1x]
node_156[stdvectorvector 10000percent 10000percent 1x]
node_157[stdvectorvector 10000percent 10000percent 1x]
node_158[void std_Destroy 10000percent 10000percent 1x]
node_159[void std_Destroy 10000percent 10000percent 1x]
node_1-->|4x|node_1
node_1-->|10000percent 3x|node_134
node_1-->|10000percent 3x|node_135
node_1-->|10000percent 3x|node_136
node_1-->|10000percent 3x|node_137
node_1-->|10000percent 1x|node_156
node_1-->|10000percent 1x|node_157
node_15-->|10000percent 8436x|node_11
node_19-->|10000percent 2271x|node_12
node_22-->|10000percent 2198x|node_12
node_23-->|10000percent 2116x|node_13
node_23-->|10000percent 2116x|node_18
node_24-->|10000percent 2116x|node_13
node_24-->|10000percent 2116x|node_23
node_25-->|10000percent 2068x|node_24
node_25-->|10000percent 48x|node_86
node_25-->|10000percent 48x|node_88
node_26-->|10000percent 1680x|node_10
node_27-->|10000percent 1456x|node_10
node_30-->|10000percent 9474x|node_10
node_30-->|10000percent 2271x|node_19
node_30-->|10000percent 1089x|node_25
node_31-->|10000percent 9500x|node_10
node_31-->|10000percent 2198x|node_22
node_31-->|10000percent 1027x|node_25
node_35-->|10000percent 772x|node_34
node_36-->|10000percent 772x|node_35
node_37-->|10000percent 201152x|node_8
node_37-->|10000percent 101472x|node_9
node_38-->|10000percent 448x|node_9
node_39-->|10000percent 448x|node_16
node_39-->|10000percent 448x|node_17
node_39-->|10000percent 448x|node_26
node_39-->|10000percent 336x|node_27
node_40-->|10000percent 448x|node_16
node_40-->|10000percent 448x|node_17
node_40-->|10000percent 224x|node_26
node_40-->|10000percent 448x|node_27
node_41-->|10000percent 448x|node_16
node_41-->|10000percent 448x|node_17
node_41-->|10000percent 336x|node_26
node_41-->|10000percent 448x|node_27
node_42-->|10000percent 448x|node_37
node_42-->|10000percent 448x|node_38
node_42-->|10000percent 448x|node_39
node_42-->|10000percent 448x|node_40
node_42-->|10000percent 448x|node_41
node_42-->|10000percent 448x|node_43
node_42-->|10000percent 448x|node_44
node_42-->|10000percent 448x|node_45
node_42-->|10000percent 448x|node_49
node_43-->|10000percent 448x|node_16
node_43-->|10000percent 448x|node_17
node_43-->|10000percent 224x|node_26
node_43-->|10000percent 224x|node_27
node_43-->|10000percent 432x|node_30
node_43-->|10000percent 163x|node_31
node_43-->|10000percent 96x|node_54
node_43-->|10000percent 51x|node_56
node_44-->|10000percent 448x|node_16
node_44-->|10000percent 448x|node_17
node_44-->|10000percent 448x|node_30
node_44-->|10000percent 448x|node_31
node_44-->|10000percent 224x|node_54
node_44-->|10000percent 224x|node_56
node_45-->|10000percent 448x|node_16
node_45-->|10000percent 448x|node_17
node_45-->|10000percent 448x|node_26
node_45-->|10000percent 209x|node_30
node_45-->|10000percent 416x|node_31
node_45-->|10000percent 97x|node_54
node_45-->|10000percent 80x|node_56
node_46-->|10000percent 448x|node_18
node_46-->|10000percent 448x|node_33
node_47-->|10000percent 448x|node_51
node_47-->|10000percent 448x|node_52
node_47-->|10000percent 448x|node_53
node_48-->|10000percent 448x|node_33
node_48-->|10000percent 448x|node_46
node_49-->|10000percent 438x|node_48
node_49-->|10000percent 10x|node_112
node_49-->|10000percent 10x|node_113
node_50-->|10000percent 448x|node_47
node_54-->|10000percent 1251x|node_10
node_54-->|10000percent 417x|node_32
node_54-->|10000percent 417x|node_36
node_54-->|10000percent 417x|node_55
node_55-->|10000percent 11818x|node_10
node_55-->|10000percent 1828x|node_15
node_55-->|10000percent 2245x|node_20
node_55-->|10000percent 2245x|node_21
node_55-->|1828x|node_55
node_56-->|10000percent 1065x|node_10
node_56-->|10000percent 355x|node_32
node_56-->|10000percent 355x|node_36
node_56-->|10000percent 355x|node_57
node_57-->|10000percent 7022x|node_10
node_57-->|10000percent 984x|node_15
node_57-->|10000percent 1339x|node_28
node_57-->|10000percent 1339x|node_29
node_57-->|984x|node_57
node_67-->|10000percent 108x|node_66
node_67-->|10000percent 108x|node_69
node_68-->|10000percent 108x|node_62
node_69-->|10000percent 108x|node_63
node_69-->|10000percent 108x|node_68
node_70-->|10000percent 108x|node_71
node_71-->|10000percent 108x|node_74
node_72-->|10000percent 108x|node_73
node_74-->|10000percent 324x|node_58
node_74-->|10000percent 108x|node_75
node_77-->|10000percent 54x|node_84
node_80-->|10000percent 54x|node_62
node_81-->|10000percent 108x|node_61
node_82-->|10000percent 216x|node_32
node_82-->|10000percent 108x|node_67
node_82-->|10000percent 54x|node_76
node_83-->|10000percent 54x|node_85
node_84-->|10000percent 54x|node_79
node_85-->|10000percent 54x|node_80
node_86-->|10000percent 54x|node_65
node_87-->|10000percent 54x|node_65
node_88-->|10000percent 48x|node_13
node_88-->|10000percent 48x|node_24
node_88-->|10000percent 96x|node_61
node_88-->|10000percent 96x|node_64
node_88-->|10000percent 96x|node_70
node_88-->|10000percent 96x|node_72
node_88-->|10000percent 48x|node_77
node_88-->|10000percent 48x|node_81
node_88-->|10000percent 48x|node_82
node_88-->|10000percent 48x|node_83
node_88-->|10000percent 48x|node_87
node_95-->|10000percent 20x|node_94
node_95-->|10000percent 20x|node_97
node_96-->|10000percent 20x|node_90
node_97-->|10000percent 20x|node_63
node_97-->|10000percent 20x|node_96
node_98-->|10000percent 20x|node_99
node_99-->|10000percent 20x|node_102
node_100-->|10000percent 20x|node_101
node_102-->|10000percent 60x|node_78
node_102-->|10000percent 20x|node_103
node_104-->|10000percent 10x|node_110
node_106-->|10000percent 10x|node_90
node_107-->|10000percent 20x|node_89
node_108-->|10000percent 40x|node_9
node_108-->|10000percent 10x|node_76
node_108-->|10000percent 20x|node_95
node_109-->|10000percent 10x|node_111
node_110-->|10000percent 10x|node_105
node_111-->|10000percent 10x|node_106
node_112-->|10000percent 10x|node_33
node_112-->|10000percent 10x|node_48
node_112-->|10000percent 20x|node_89
node_112-->|10000percent 20x|node_92
node_112-->|10000percent 20x|node_98
node_112-->|10000percent 20x|node_100
node_112-->|10000percent 10x|node_104
node_112-->|10000percent 10x|node_107
node_112-->|10000percent 10x|node_108
node_112-->|10000percent 10x|node_109
node_112-->|10000percent 10x|node_114
node_113-->|10000percent 10x|node_93
node_114-->|10000percent 10x|node_93
node_115-->|10000percent 6x|node_18
node_115-->|10000percent 6x|node_91
node_118-->|10000percent 6x|node_116
node_119-->|10000percent 6x|node_117
node_121-->|10000percent 6x|node_118
node_121-->|10000percent 6x|node_123
node_122-->|10000percent 6x|node_119
node_124-->|10000percent 6x|node_121
node_125-->|10000percent 6x|node_77
node_125-->|10000percent 6x|node_122
node_126-->|10000percent 6x|node_91
node_126-->|10000percent 6x|node_115
node_127-->|10000percent 6x|node_86
node_127-->|10000percent 6x|node_91
node_127-->|10000percent 6x|node_128
node_128-->|10000percent 12x|node_61
node_128-->|10000percent 12x|node_64
node_128-->|10000percent 12x|node_70
node_128-->|10000percent 12x|node_72
node_128-->|10000percent 6x|node_77
node_128-->|10000percent 6x|node_81
node_128-->|10000percent 6x|node_82
node_128-->|10000percent 6x|node_83
node_128-->|10000percent 6x|node_87
node_128-->|10000percent 6x|node_91
node_128-->|10000percent 6x|node_126
node_129-->|10000percent 6x|node_11
node_129-->|10000percent 6x|node_127
node_130-->|10000percent 6x|node_124
node_131-->|10000percent 6x|node_64
node_131-->|10000percent 6x|node_125
node_131-->|10000percent 6x|node_133
node_132-->|10000percent 6x|node_120
node_133-->|10000percent 6x|node_132
node_134-->|10000percent 3x|node_129
node_134-->|10000percent 3x|node_130
node_135-->|10000percent 3x|node_131
node_136-->|10000percent 3x|node_129
node_136-->|10000percent 3x|node_130
node_137-->|10000percent 3x|node_131
node_138-->|10000percent 2x|node_140
node_138-->|10000percent 2x|node_141
node_138-->|10000percent 2x|node_142
node_139-->|10000percent 2x|node_138
node_143-->|10000percent 1x|node_1
node_143-->|10000percent 1x|node_145
node_144-->|10000percent 3357x|node_14
node_144-->|10000percent 448x|node_42
node_144-->|10000percent 448x|node_50
node_144-->|10000percent 320x|node_59
node_144-->|10000percent 320x|node_60
node_144-->|10000percent 2x|node_139
node_148-->|10000percent 1x|node_146
node_149-->|10000percent 1x|node_147
node_151-->|10000percent 1x|node_148
node_151-->|10000percent 1x|node_153
node_152-->|10000percent 1x|node_149
node_154-->|10000percent 1x|node_151
node_155-->|10000percent 1x|node_104
node_155-->|10000percent 1x|node_152
node_156-->|10000percent 1x|node_154
node_157-->|10000percent 1x|node_92
node_157-->|10000percent 1x|node_155
node_157-->|10000percent 1x|node_159
node_158-->|10000percent 1x|node_150
node_159-->|10000percent 1x|node_158
```

# Discussion

Using a sliding window based method is possible since instead of waiting
for the data to arrive and accumulate it, we could move over the data
and calculate accordingly. Irrelevant parts of data structures could be
pruned which is not possible at a sorting based approach.

Though heaps provided immense speed in finding the quartiles and the
median, it seemed redundant to use three pairs of heaps to provide
related information about the same data. Though the memory complexity is
constantly growing, if the memory would like to be optimised against
speed an approach combining all three structures is possible. The
structure requires a special type of heap called min-max heap which is a
heap slightly more complicated to use but stores both min and max of the
data. By using this heap it is possible to find the first quartile,
median and third quartile from a single entity. To build the structure
it would be needed to have 4 heaps of same size. From the minimum to the
first quartile of the data can be stored in a max heap and the data from
the maximum of data to its third quartile can be stored in the min heap.
For the remaining two heaps it would be needed to use the minmax heap
discussed above to use the same heaps for both median and its
neighbouring quartile. Some implementation of minmax heap can be found
in my heap.h file. The problem with this structure is that the addition
of elements and the state protection have to be thought thoroughly. A
sift in one heap may trigger the next heap to sift. To sum up, less
memory can be used by having a more complicated data structure and
management system.
