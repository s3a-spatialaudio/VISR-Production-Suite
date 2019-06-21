.. _object_based_audio:

Object-based audio
-------------------

What is it?
^^^^^^^^^^^

A novel paradigm and technology to produce and distribute audio, which allows an audio content creator to target different playback systems and formats using a single production. Therefore you can create scalable productions, being also future-proof by being able to easily support future formats.

  .. figure:: ../images/object-based.png
      :align: center
      :scale: 50 %
      :alt: alternate text

      object-based production

.. _how_does_it_work:

How does it work?
^^^^^^^^^^^^^^^^^
The fundamental elements of an object-based production are:

* **Objects**: audio objects are audio **signals** with associated **metadata**. The metadata of all the objects together form the
* **Scene**: a way to represent the metadata of the set of objects, which is neutral in regard to the output audio format and system. Those information are sent to the
* **Renderer**: a component that uses the scene **metadata** to mix the **signals** of the objects in order to generate the output audio signals for different types of devices (Loudspeakers, Headphones, ...) and layouts ( stereo, binaural, 5.1, 7.1, 22.1, ...).

An important feature of object based is that having an all-purpose **scene** representation means that the rendering part can be in the DAW as usual, or postponed in any further step of the audio chain, including the end user device.
The latter scenario paves the way to new possibilities in the **interaction** and **personalisation** of the audio content, e.g. allowing the end user to adjust the singer's voice only, while listening to a song on a smartphone.

Channel-based vs object-based
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This is a quick comparison between the channel-based approach and the object-based one:

+------------------------------------------------+----------------------------------------+
| channel-based   (Fig.1)                        | object-based (Fig.2)                   |
+================================================+========================================+
| Production for one format only                 |  Scalable and future proof productions |
+------------------------------------------------+----------------------------------------+
| No adaptation possible after mix               |  New end user experiences:             |
|                                                |  interaction and personalisation       |
+------------------------------------------------+----------------------------------------+

  .. figure:: ../images/channel-based.png
      :align: center
      :scale: 80 %
      :alt: alternate text

      **Fig.1** typical channel-based production

  .. figure:: ../images/object-based.png
      :align: center
      :scale: 50 %
      :alt: alternate text

      **Fig.2** object-based production

See the following webpages for more info on object-based audio: 

https://lab.irt.de/demos/object-based-audio/

https://www.bbc.co.uk/rd/object-based-media
