module 'testing' {
   lib {
      pch_src 'src/pch.cpp',
      define 'BE_TESTING_IMPL'
   },
   app '-test' {
      icon 'icon/bengine-test-perf.ico',
      link_project {
         'testing',
         'core',
         'core-id',
         'util'
      }
   }
}
