module { name = 'testing',
   projects = {
      lib {
         src = {
            'src/*.cpp',
            pch = 'src/pch.cpp'
         },
         preprocessor = {
            'BE_TESTING_IMPL'
         }
      },
      app { suffix = 'test',
         icon = 'icon/bengine-test-perf.ico',
         src = 'test/*.cpp',
         libs = {
            'testing',
            'core',
            'core-id',
            'util'
         },
      }
   }
}
