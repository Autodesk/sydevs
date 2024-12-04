# Contributing to SyDEVS

We're delighted that you'd like to contribute to this project. Please review the guidelines below.

## Contributor License Agreement

Before contributing any code to this project, we kindly ask you to sign a Contributor License Agreement (CLA). We can not accept any pull request if a CLA has not been signed.

- If you are contributing on behalf of yourself, the CLA signature is included as a part of the pull request process.

- If you are contributing on behalf of your employer, please sign our [Corporate Contributor License](https://github.com/Autodesk/autodesk.github.io/releases/download/1.0/ADSK.Form.Corp.Contrib.Agmt.for.Open.Source.docx) Agreement. The document includes instructions on where to send the completed forms to. Once a signed form has been received, we can happily review and accept your pull requests.

Contributors are expected to follow the [Contributor Covenant](CODE_OF_CONDUCT.md), which describes the code of conduct for Autodesk open source projects like this one.

## Communicating possible changes

To request a feature or report a bug, please open an issue. Also, feel free to email the lead developer, Rhys Goldstein <rhys.goldstein@autodesk.com>, to discuss any of the following:

- Contributing a new example or simulation
- Contributing to an existing example or simulation
- Contributing to the underlying framework
- Exploring whether the framework is suitable for your application
- Exploring possible enhancements to the framework

## Submitting a pull request

In general, contributors should develop on branches based off of `main` and pull requests should be made against `main`. To submit a pull request, follow the procedure below:

1. Fork and clone the repository.
1. Create a new branch based on `main`: `git checkout -b <my-branch-name> main`.
1. Make your changes, ensure the existing tests still pass; if needed add new tests and make sure they pass.
1. Push to your fork and submit a pull request from your branch to `main`.

Here are a few things you can do that will increase the likelihood that your pull request will be accepted:

- Follow the existing style where possible.
- Keep your change as focused as possible.
  If you want to make multiple independent changes, please consider submitting them as separate pull requests.
- Write a [good commit message](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html).

## Adding a new unit test

The unit testing framework used in SyDEVS is [Catch2](https://github.com/catchorg/Catch2). See the [documentation](https://github.com/catchorg/Catch2/tree/master/docs) for a tutorial and reference material. Catch2 is released under the Boost Software License 1.0. The steps below will add a new unit test:

1. `cd test/unit_tests`
1. Create a new source file (e.g. `test_some_file.cpp`) or open the appropriate existing file in your editor.
1. Add code similar to the following:

   ```
    #include <catch2/catch.hpp>

    TEST_CASE("test name goes here")
    {
        CHECK(2 == 3);
    }
   ```

1. Save the test file.
1. `cd ../../bin`
1. `cmake ..`
1. `make`

## Adding a new regression test

The regression tests check that each selected simulation example produces the expected output. To add a new example to the regression tests, the following changes are needed:

1. Edit `CMakeLists.txt` to compile a new example.
1. Edit `test/regression_tests/main.cpp` to include the new example.
1. Add the expected output to `test/regression_tests/regression_test_data` in a new text file.

## Documenting the Code

We are using [Doxygen](http://www.stack.nl/~dimitri/doxygen/) to build our documentation and have adopted the following policies.

- Use JavaDoc keywords. For example:

  ```
  /**
   * @brief A brief description goes here.
   *
   * @details A detailed description goes here.
   *
   * @param foo A description of the argument goes here.
   *
   * @return A description of the return value goes here.
   */
  ```

- Put class documentation before the class definition. For example:

  ```
  /**
   * @brief A brief description of stay_class goes here.
   *
   * @details
   * A much more detailed description of stay_classy goes here.
   */
  class stay_classy
  {
      ...
  };
  ```

- Put method documentation before the method declaration. For short descriptions it is acceptable to place the documentation at the end of the statement with `///<`.

  ```
  /**
   * @brief A brief description of do_something goes here.
   *
   * @details
   * A detailed description of do_something goes here.
   *
   * @param foo A description of argument goes here.
   * @param bar A description of argument goes here.
   *
   * @return A description of the return value goes here.
   */
  int64 do_something(int64 foo, float64 bar);
  ```
  
  or
   
  ```
  int64 do_something(int64 foo, float64 bar);  ///< This is a short description
  ```
  
- To create a list in a description use the dash markdown notation. For example:

  ```
  /**
   *
   * Here's a list:
   *
   * - Item A.
   * - Item B.
   * - Item C.
   */
  ```
  
- To highlight parameters in brief or detailed descriptions use back-tick markdown notation. For example:
  
  ```
  /**
   * @details
   * Draw attention to the `foo` and `bar` arguments.
   */
  ```
  
- To mark source in a description use tilde-fence markdown notation. For example:
  
  ```
  /**
   * Example code:
   *
   * ~~~
   * int64 count = 0;
   * for (int64 index = 0; index < count; ++index) {
   *     // do something...
   * }
   * ~~~
   */
  ```
