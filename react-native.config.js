/**
 * @type {import('@react-native-community/cli-types').UserDependencyConfig}
 */
module.exports = {
  dependency: {
    platforms: {
      android: {
        cxxModuleCMakeListsModuleName: 'react-native-opus',
        cxxModuleCMakeListsPath: 'src/main/jni/CMakeLists.txt',
        cxxModuleHeaderName: 'NativeOpusTurboModule',
      },
    },
  },
};
