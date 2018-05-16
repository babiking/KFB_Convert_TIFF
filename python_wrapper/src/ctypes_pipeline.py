# !Created by babiking@sensetime on May 16th, 2018 for ctype tutorial
#  Reference: https://docs.python.org/fr/3/library/ctypes.html

from ctypes import *

# ! Step 1: Load Dynamic Link Library
'''
    Method 1: ctypes.cdll.LoadLibrary('.so.x')
    Method 2: create an instance of CDLL by calling the constructor i.e. CDLL('.so.x')
    
    libraries: attributes of objects e.g. cdll, windll and oledll
'''

libc = cdll.LoadLibrary('libc.so.6')   # or libc = CDLL('libc.so.6')



# ! Step 2: Access the function list
'''
    cdll:   __cdecl calling convention i.e. C Declaration (parameters pushed into stack from right to left, at the end of calling, stack cleared automatcally by retnX
                                                           note: X- number of BYTES in stack)
    windll: _stdcall calling convention i.e. StandardCall (+this -> class parameter)
        export function in ANSI or UNICODE version:
            [1] ANSI: GetModuleHandleA e.g. print(windll.kernel32.GetModuleHandleA)
            [2] UNICODE: GetModuleHandleW
    
    windows:
        [1] GetModuleHandleA or GetModuleHandleW -> return ANSI or UNICODE version export
            Or exported not by function name but by ordinal
                e.g. cdll.kernel32[0] VS windll.kernel32.GetModuleHandleA
        [2] getarr -> retrieve functions from invalid python identifier
                e.g. getattr(cdll.msvcrt, "??2@YAPAXI@Z")

'''

# ! Step 3: Calling functions
'''
    ValueError: calling convention error e.g. call an stdcall function with cdecl convention, vice versa
    
    tool: faulthandler module
            faulthandler.enable() to install fault handlers for SIGSEGV, SIGFPE, SIGABRT, SIGBUS and SIGILL signals
'''
print(libc.time(None)) # None-NULL Pointer

'''
    ctypes argument conversion allows instance of custom class by looking for _as_parameter_ attributes
        implement class method from_param() to receive Python object passed to function call: 
            [1]TypeCheck then return the object itself, its _as_parameter_ attribute or whatever passed as C function argument
            [2]Return of from_param() should be int, string, byte, ctype instance or an object with _as_parameter attribute
'''
printf = libc.printf

class Bottles:
    def __init__(self, number):
        self._as_parameter_ = number

bottles = Bottles(42)
printf(b"%d bottles of beer\n", bottles)

'''
    Or the argument can be defined as a property i.e. an attribute available on request in custom class
    reference: https://www.liaoxuefeng.com/wiki/001374738125095c955c1e6d8bb493182103fac9270762a000/001386820062641f3bcc60a4b164f8d91df476445697b9e000
'''
class Bottles_property:
    def __init__(self):
        self._number = 42

    @property
    def number(self):
        return self._number

    @number.setter
    def number(self, value):
        if isinstance(value, int):
            self._number = value
        else:
            raise ValueError('number of beer bottles should be integer!')
bottles = Bottles_property()
bottles.number = 16
printf(b"%d bottles of beer\n", bottles.number)



'''
    Function prototype: 
        1. required argument types
        2. return types
'''
printf.argtypes = [c_char_p, c_char_p, c_int, c_double]
printf(b"String '%s', Int %d, Double %f\n", b"Hi", 10, 2.2)



'''
    Passing pointers or passing parameters by reference to function call
    i.e. byref() and pointer()
'''
i = c_int()
f = c_float()
s = create_string_buffer(b'\000'*32)
print(i.value, f.value, repr(s.value))   # repr(object) return the string-format object to Python interpreter
sscanf = libc.sscanf
sscanf(b"1 3.14 Hello", b"%d %f %s", byref(i), byref(f), s)
print(i.value, f.value, repr(s.value))
sscanf(b"3 5.12 World", b"%d %f %s", pointer(i), pointer(f), s)
print(i.value, f.value, repr(s.value))



'''
    Passing Structures and unions
        class with _fields_ attribute i.e. a list of 2-tuples containing a field name and a field type
        byte-order: BigEndian, LittleEndian note. usually the native machine byte order
'''
class POINT(Structure):
    _fields_ = [("x", c_int), ("y", c_int)]
point = POINT(10, 20)
print(point.x, point.y)

class RECT(Structure):
    _fields_ = [("upperleft", POINT), ("lowerright", POINT)]
rc = RECT(point)
print(rc.upperleft.x, rc.upperleft.y)
print(rc.lowerright.x, rc.lowerright.y)



'''
    Callback functions i.e. create C callable function pointers from Python callable objects
'''
intArray5 = c_int*5
ia = intArray5(5, 1, 7, 33, 99)
qsort = libc.qsort
qsort.restype = None

# !Create a class of callback function with known informations: calling convention, return type and the number and types of arguments
CMPFUNC = CFUNCTYPE(c_int, POINTER(c_int), POINTER(c_int))
def py_cmp_func(a, b):
    print("py_cmp_func", a[0], b[0])
    return a[0] - b[0]

cmp_func = CMPFUNC(py_cmp_func)
# !qsort is a C-Library function, cdecl calling convention
qsort(ia, len(ia), sizeof(c_int), cmp_func)

for i in ia: print(i, end=" ")