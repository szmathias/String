<!-- Improved compatibility of back to top link: See: https://github.com/othneildrew/Best-README-Template/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<!-- [![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<div align="center">
<h2 align="center">String</h2>
  <p align="center">
    A simple string library
    <br />
    <a href="https://github.com/szmathias/String/issues">Report Bug</a>
    ·
    <a href="https://github.com/szmathias/String/issues">Request Feature</a>
  </p>
</div>

<!-- TABLE OF CONTENTS -->
## Table of Contents
- [About the Project](#about-the-project)
- [Getting Started](#getting-started)
  - [Specifications](#specifications)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)
- [Acknowledgments](#acknowledgments)

<!-- ABOUT THE PROJECT -->
## About the Project

I created this library for some of my other projects and decided to share it here! This is my attempt to implement small string optimization based off of C++ strings. If anyone is interested in my tests please feel reach out and I'll commit those as well.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- GETTING STARTED -->
## Getting Started

Download `String.h` and `String.c` then include them just like any other C file.

### Specifications

I've only tested it on my home computer with:
- Ubuntu 22.04
- gcc 11.3.0
- c17 standard

Please feel free to test it outside of that environment and let me know how it works!

<!-- USAGE EXAMPLES -->
## Usage

Simple Hello World:
```c
#include <stdio.h>
#include "String.h"

int main(int, char**)
{
  String hello = str_create("Hello Strings!");
  printf("%s\n", str_data(&hello));
  str_free(&hello);
  return 0;
}
```

Using `STR_NPOS` to find a word in the string:
```c
#include <stdio.h>
#include "String.h"

int main(int, char**)
{
  String str = str_create("sausages");
  size_t found = str_find_str(&str, "sage");
  if(found != STR_NPOS)
  {
    printf("It's here!\n");
  }
  else
  {
    printf("We need to keep looking...\n");
  }
  str_free(&str);
  return 0;
}
```

In the header file `STR_MIN_INIT_CAP` is used for the SSO. Memory won't be allocated until that is exceeded. Feel free to change it to your needs:
```c
#include <stdio.h>
#include "String.h"

int main(int, char**)
{
  String str = str_create("No memory here");
  str_push_back(&str, ". Whoops we went too far!");
  printf("%s\n", str_data(&str));
  str_free(&str);
  return 0;
}
```
<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Zack Mathias - szmathias@gmail.com

Project Link: [https://github.com/szmathias/String](https://github.com/szmathias/String)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

* [othneildrew for this awesome README template!](https://github.com/othneildrew/Best-README-Template)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
<!-- [contributors-shield]: https://img.shields.io/github/contributors/szmathias/String.svg?style=for-the-badge
[contributors-url]: https://github.com/szmathias/String/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/szmathias/String.svg?style=for-the-badge
[forks-url]: https://github.com/szmathias/String/network/members
[stars-shield]: https://img.shields.io/github/stars/szmathias/String.svg?style=for-the-badge
[stars-url]: https://github.com/szmathias/String/stargazers
[issues-shield]: https://img.shields.io/github/issues/szmathias/String.svg?style=for-the-badge
[issues-url]: https://github.com/szmathias/String/issues
[license-shield]: https://img.shields.io/github/license/szmathias/String.svg?style=for-the-badge
[license-url]: https://github.com/szmathias/String/blob/master/LICENSE.txt -->
