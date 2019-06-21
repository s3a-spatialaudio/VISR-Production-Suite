Copyright (c) 2014-2019 Giacomo Costantini giacomo.costantini3@gmail.com--- All rights reserved.
Copyright (c) 2014-2019 Institute of Sound and Vibration Research, University of Southampton and VISR project contributors --- All rights reserved.

# Overview

The VISR Production Suite is the first open-source set of DAW plugins for producing and reproducing object-based audio.

It consists of an audio object editor to transform your tracks into audio objects, a scene editor to manipulate the whole object-based audio scene, and integrated renderers to reproduce the audio scene on various loudspeaker layouts, sound bars and headphones.

What you can do with the VISR Production Suite:

* Create an object-based session

* Spatialise and control your audio objects

* Easy binaural content creation with different sets of HRIR and BRIRs in SOFA format

* Head tracking and head orientation compensation capabilities, suitable for VR

* Create loudspeaker layout agnostic spatial audio content and render it for different loudspeaker systems with just one click

* Exploit JUCE framework and the VISR, a modular open-source framework for audio processing

Development of this project has been supported by the EPSRC Programme Grant S3A: Future Spatial Audio for an Immersive Listener Experience at Home (EP/L000539/1) http://www.s3a-spatialaudio.org and the BBC as part of the BBC Audio Research Partnership.

Find more at: https://cvssp.org/data/s3a/public/VISRPluginSuite/html/index.html or using the .pdf user document in "doc/" folder.

Website: http://www.s3a-spatialaudio.org/plugins

# Contributors

Specific parts of the software have been provided by other groups as listed in the file Contributors.md

# Contributing

Contributions to the project are welcome -- through comments, feature requests bug reports or code contributions.

## Bug reports and feature requests

Please submit feature recommendations and bugs through GitHub's issue tracking system.

# Code contributions

Code contributions are handled through GitHub pull requests.
Please see [this article](https://help.github.com/articles/about-pull-requests/) for an introduction to pull requests.

Contributors are required to sign a Contributor License agreement (CLA).
This is to ensure that all contributions can be distributed under the same licence terms as the original project.
For more information, see for example this [article](http://oss-watch.ac.uk/resources/cla).
CLA templates can be found in the `doc/contributing` subdirectory of the repository.

# License

The software is provided under the GPL Version 3 license printed below and in the Licence.md file

GNU GENERAL PUBLIC LICENSE Version 3
https://www.gnu.org/licenses/gpl.txt

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Licences of third-party software

This software uses the following external software components subject to the following licenses:

# The JUCE 5 Library

**BY DOWNLOADING, INSTALLING OR USING ANY PART OF THE JUCE LIBRARY, YOU AGREE
TO THE [JUCE 5 END-USER LICENSE AGREEMENT](https://www.juce.com/juce-5-licence)
AND [JUCE 5 PRIVACY POLICY](https://www.juce.com/juce-5-privacy-policy), WHICH
ARE BINDING AGREEMENTS BETWEEN YOU AND ROLI, LTD. IF YOU DO NOT AGREE TO THE
TERMS, DO NOT USE THE JUCE LIBRARY.**

We now have tier-leveled license terms for JUCE 5, with different terms for
each available license: JUCE Personal (for developers or startup businesses
with revenue under 50K USD "Revenue Limit"; free), JUCE Indie (for small
businesses with under 200K Revenue Limit; $35/month), JUCE Pro (no Revenue
Limit; $65/month), and JUCE Educational (no Revenue Limit; free for bona fide
educational institutes). All licenses allow you to commercially release
applications so long as you do not exceed the Revenue Limit and pay applicable
Fees. Once your business hits the Revenue Limit for your JUCE license, you will
either have to upgrade your JUCE license or release your Applications under the
[GNU General Public License v.3](https://www.gnu.org/licenses/gpl-3.0.en.html),
which means, among other things, that your code can be freely copied and
distributed.

You agree to give notice to the end-users of your Applications that we may
track the IP addresses associated with their use of the Applications using JUCE
solely for our internal purposes in providing JUCE, unless you are a paying
JUCE customer and opt-out of such tracking. You agree to fully comply with all
laws, including relating to the collection of information from children and the
[Children’s Online Privacy Protection Act
 (COPPA)](https://www.ftc.gov/enforcement/rules/rulemaking-regulatory-reform-proceedings/childrens-online-privacy-protection-rule).

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE, ARE DISCLAIMED.

The juce_audio_basics, juce_audio_devices, juce_blocks_basics, juce_core and
juce_events modules are permissively licensed under the terms of the [ISC
                                                                      license](http://www.isc.org/downloads/software-support-policy/isc-license).

For more information, visit the website:
[www.juce.com](https://www.juce.com)

FULL JUCE TERMS:
- [JUCE 5 END-USER LICENSE AGREEMENT](https://www.juce.com/juce-5-licence)
- [JUCE 5 PRIVACY POLICY](https://www.juce.com/juce-5-privacy-policy)

## VISR Framework

ISC (Internet Systems Consortium) License
https://www.isc.org/downloads/software-support-policy/isc-license/

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.

## Boost

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

## SNDFILE

Copyright (C) 1999-2011Erik de Castro Lopo <erikd@mega-nerd.com>

Project page: http://www.mega-nerd.com/libsndfile
License statement: http://www.mega-nerd.com/libsndfile/#Licensing

libsndfile is released under the terms of the GNU Lesser General Public License, of which there are two versions; version 2.1 (http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html) and version 3 (http://www.gnu.org/copyleft/lesser.html). To maximise the compatibility of libsndfile, the user may choose to use libsndfile under either of the above two licenses.

Sndfile libraries uses the FLAC, OGG and VORBIS libraries, whose licenses are shown below.

## FLAC - Free Lossless Audio Codec

https://xiph.org/flac/license.html

Copyright (C) 2001,2002,2003,2004,2005,2006,2007  Josh Coalson

FLAC is comprised of several
components distributed under difference licenses.  The codec libraries
are distributed under Xiph.Org's BSD-like license (see the file
COPYING.Xiph in this distribution).  All other programs, libraries, and
plugins are distributed under the LGPL or GPL (see COPYING.LGPL and
COPYING.GPL).  The documentation is distributed under the Gnu FDL (see
COPYING.FDL).  Each file in the FLAC distribution contains at the top the
terms under which it may be distributed.
Since this particular file is relevant to all components of FLAC,
it may be distributed under the Xiph.Org license, which is the least
restrictive of those mentioned above.  See the file COPYING.Xiph in this
distribution.


## VORBIS

https://svn.xiph.org/trunk/vorbis/COPYING

Copyright (c) 2002-2015 Xiph.org Foundation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

- Neither the name of the Xiph.org Foundation nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



## OGG

https://svn.xiph.org/trunk/ogg/COPYING

Copyright (c) 2002, Xiph.org Foundation

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

- Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

- Neither the name of the Xiph.org Foundation nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
