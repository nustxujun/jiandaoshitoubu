
cpp.test0()
cpp.test1(10)
cpp.test6(true)

print(cpp.test13(false, 1, "test"))

func = function() print("111") end

print(cpp.test15("test123", func, 10))

handler = function(n, m) 
	print("handler")
	return true, "", 11, "response msg data"
end

cpp.test16(handler)

local print_func = function(t) print(t) end
cpp.test17(print_func, "haha")

local func_func = function(func) func() end

cpp.test18(func_func, func)

print(cpp.test19(2323, "test"))

print(cpp.test20())


 ff = foo_t:new(1);
-- --ff1 = foo_t:new(10);
ff:add(1, 4)

 sub_ff = ff:get_pointer()


 --base = sub_ff:get_base()
 --base:print()

base_print()
base_cacu(10)
base_lambda(21)
-- print(test_namespace.test20())