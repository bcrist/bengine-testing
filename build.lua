module 'testing' {
   lib {
      src {
         'src/*.cpp',
         pch_src 'src/pch.cpp'
      },
      define 'BE_TESTING_IMPL',
      link_project 'core-id-with-names'
   },
   app '-test' {
      icon 'icon/bengine-test-perf.ico',
      link_project {
         'testing',
         'util'
      }
   }
}
