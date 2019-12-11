project('aoc2019', 'c')

cc = meson.get_compiler('c')
m = cc.find_library('m', required: true)

args = [ '-std=c90', '-pedantic-errors', '-Wall', '-Wconversion' ]

executable('1'       , '1/main.c'  , c_args: args , dependencies: [ m ])
executable('2'       , '2/main.c'  , c_args: args)
executable('3_part1' , '3/part1.c' , c_args: args)
executable('3_part2' , '3/part2.c' , c_args: args)
