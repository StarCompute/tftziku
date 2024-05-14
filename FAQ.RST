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

    getPixDataFromStr("业精于勤荒于嬉戏，行成于思毁于随",_font_lib_name)



如何调整字库内容
************************************************************************

个人强烈建议你使用完整的字库，如果实在不愿意使用，只想创建少数汉字的字库，可以直接修改

.. code-block::  python

    # FONT_ALL = "清澈的爱只为中国1"

中的内容，反注释调上面这句话，调整对应的文字为你需要的内容，然后重新运行python就可以了。
使用这种方法创建字库，比其他现在的字库管理都更方便，少了无数繁琐的过程。
如果你使用PCtoLCD2018 这种方案，要进行各种数据整理，哪怕你只是调整一个字。



如何创建一个16号的 “微软雅黑 细体” 字库
************************************************************************

调整以下代码

.. code-block::  python

    # 下面代码模式设置字号，可以按照偶数进行调整，但是都必须大于12，12以下的显示不全
    _font_size=12

    # 如果打算使用其他字体请修改下面的simsun.ttc,例如微软雅黑是msyh.ttc ，具体请查看windows内fonts目录内的字体。
    _font_name="simsun.ttc"

为

.. code-block::  python

    # 下面代码模式设置字号，可以按照偶数进行调整，但是都必须大于12，12以下的显示不全
    _font_size=16

    # 如果打算使用其他字体请修改下面的simsun.ttc,例如微软雅黑是msyh.ttc ，具体请查看windows内fonts目录内的字体。
    _font_name="msyhl.ttc"



然后运行python文件就可以了。



如何自动上传创建的字库文件
************************************************************************

手动上传字库文件到esp单片机是个很麻烦的事情，所以注释掉getunicodeV3.py文件下面这段代码可以运行，就可以在字库创建后自动上传字库到单片机中

.. code-block::  python

    # import os
    # os.system("cd ..")
    # os.system("move  "+_font_lib_name+" data\\x.font")
    # 请注意检查下面pio.exe 的配置地址和--environment 后esp32dev 应该为你的配置，这个配置就是你platform.ini中的[env:xxx]中这个xxx
    # os.system("C:\\Users\\xuank\\.platformio\\penv\\Scripts\\pio.exe  run --target uploadfs --environment nodemcuv2")


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



直接使用已经生成的字库
************************************************************************

以下字体分别创建了字库放在了font目录中

simsun.ttc      宋体

simfang.ttf     仿宋

simkai.ttf      楷体

simhei.ttf      黑体

msyh.ttc        微软雅黑


并且已经创建了12，14，16和18号字体，都可以拷贝到data目录下命名为x.font ，上传到单片机直接调用

.. 注意:: 字号的选择：

    由于不同字体本身的特征，有些字体不适合选择16号以下的字号，虽然说有生成。



字体的漂移
************************************************************************

由于不同的字体本身的特征，字库在创建的时候会存在部分漂移的情况，显示不完整，这种请留言联系



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



PIL库停止维护问题
************************************************************************

python 文件 getunicodeV3.py 使用了PIL库，请安装 Pillow 库，兼容



不同屏幕驱动的配置调整
************************************************************************

在 xfont.c下面的代码中

.. code-block:: c



    #ifdef ARDUINO_GFX

        Arduino_DataBus *bus = create_default_Arduino_DataBus();
        Arduino_GFX *tft = new Arduino_ST7735(bus);
        #define GFX_BL DF_GFX_BL

    #elif defined(TFT_ESPI)
        TFT_eSPI tft = TFT_eSPI();
    #endif    


调整不同的配置支持不同的TFT驱动芯片，Arduino_GFX 需要修改上面的 Arduino_ST7735， 在TFT_eSPI中需要调整那个user_select.ini文件。
有兴趣的朋友也可以参考 https://github.com/StarCompute/tftziku/issues/3 中这位朋友的方式。


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

非常感谢大家使用这个开源项目，希望可以为你带来帮助；但是我不是万能的，不可能知道你在做什么和如何做
有人留言如下 ：

.. code-block:: 

   想问一下为什么总是显示库有问题呀

.. code-block:: 


    复制你的代码，编译代码出错，为什么😳

这种不清不楚的留言，哪怕我有百分百诚意，也是无法提供帮助的。说的基础一点至少要提供编译或者运行时错误才能知道发生了啥。



致谢
************************************************************************

justdomyself 