这里把大家常见的问题汇总于此
##############################################


如何创建字库
************************************************************************

直接运行getunicodeV3.py文件就可以创建你的字库了

文件中下面的代码创建字库

.. code-block::  python

    createFont( FONT_ALL,"P", _font_name, _font_size,_bin_type)

文件中下面的代码验证创建的字库成功并显示：

.. code-block::  python

    getPixDataFromStr("业精于勤荒于嬉戏，行成于思毁于随","font/x_f"+str(_font_size)+"_b"+str(_bin_type)+".font")


如何调整字库内容
************************************************************************

个人强烈建议你使用完整的字库，如果实在不愿意使用，只想创建少数汉字的字库，可以直接修改

.. code-block::  python

    # FONT_ALL = "清澈的爱只为中国1"

中的内容，反注释调上面这句话，调整对应的文字为你需要的内容，然后重新运行python就可以了。
使用这种方法创建字库，比其他现在的字库管理都更方便，少了无数繁琐的过程。


如何自动上传创建的字库文件
************************************************************************

手动上传字库文件到esp单片机是个很麻烦的事情，所以注释掉getunicodeV3.py文件下面这段代码可以运行，就可以在字库创建后自动上传字库到单片机中

.. code-block::  python

    import os
    os.system("cd ..")
    os.system("move  font\\x_f"+str(_font_size)+"_b"+str(_bin_type)+".font data\\x.font")
    # 请注意检查下面pio.exe 的配置地址和--environment 后esp32dev 应该为你的配置，这个配置就是你platform.ini中的[env:xxx]中这个xxx
    os.system("C:\\Users\\xuank\\.platformio\\penv\\Scripts\\pio.exe  run --target uploadfs --environment nodemcuv2")


注意：1，调整上面这段代码中pio.exe 的文件位置；2，调整nodemcuv2 为你的配置


如何修改字体
************************************************************************

修改创建文件中的 ``simsun.ttc `` 为其他字体的名字，如果你使用windows，请到对应的fonts目录进行查找字库名称；

.. code-block::  
    
    _font_name="simsun.ttc"


如何修改字号
************************************************************************

修改下面代码中对应的数字即可。

.. code-block::  

    _font_size=12


明明上传字库成功，为啥还提示我找不到字库 
************************************************************************

可能littleFS没有正常工作, 可能运行报错：

.. code-block:: 

    esp_littlefs: mount failed, (-84)
    esp_littlefs: Failed to initialize LittleFS
    begin(): Mounting LittleFS failed! Error: -1
    open(): File system is not mounted


需要在platformio.ini中配置

.. code-block:: 

    board_build.filesystem = littlefs


修改上面的参数后需要重新上传字库，并且编译代码上传。


显示为什么是倾倒的
************************************************************************

因为个人习惯，tft屏幕显示设置了旋转，可以自行调整  ``tft->setRotation(1);  `` .



如何让你的platformio 运行的更高效
************************************************************************

开启platformio的缓存模式，在platformio.ini中加入：

.. code-block:: 

    [platformio]
    build_cache_dir	=	cache

然后，platformio就会用磁盘换性能，编译过的文件将不在编译，极大的提高性能。


什么样的提问我会不予理睬
************************************************************************

感谢大家使用这个开源项目，但是我不是万能的，不可能知道你在做什么和如何做
有人留言如下 ：

.. code-block:: 

   想问一下为什么总是显示库有问题呀

.. code-block:: 


    复制你的代码，编译代码出错，为什么😳

这种不清不楚的留言，哪怕我有百分百诚意，也是无法提供帮助的。说的基础一点至少要提供编译或者运行时错误才能知道发生了啥。
