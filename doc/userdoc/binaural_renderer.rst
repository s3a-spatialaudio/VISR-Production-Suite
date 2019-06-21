Binaural Renderer
""""""""""""""""""

.. image:: ../images/BinauralRenderer.png
   :scale: 40 %

| With the **Binaural Renderer** you will be able to easily create **binaural** content. It allows you to switch between different SOFA-compliant **HRIR** and **BRIR** datasets and to try different binaural rendering approaches.
| Moreover this plugin is capable of receiving head orientation information through OSC to exploit dynamic binaural cues by applying head rotation compensation. This makes it the perfect audio tool for **VR**.
| The plugin has been made as a wrapper around the functionalities of the existing **VISR BST (Binaural Synthesis Toolkit)**.

**Features:**

* Listen to your object-based **scene** with just a pair of headphones
* Explore different binaural rendering approaches in the same plugin
* Use different datasets of HRIR and BRIRs under the form of SOFA files
* Head tracking and head orientation compensation through usage of **Razor AHRS 9DOF** tracker
* Head tracking and head orientation compensation through **OSC** using external trackers
* Use the python package **VISR BST (Binaural Synthesis Toolkit)** inside a DAW Plugin
