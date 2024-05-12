这里把大家常见的问题汇总于此
##############################################


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
    lib_deps = lorol/LittleFS_esp32@^1.0.6 


修改上面的参数后需要重新上传字库，并且编译代码上传。


显示为什么是倾倒的
************************************************************************

因为个人习惯，tft屏幕显示设置了旋转，可以自行调整  ``tft->setRotation(1);  `` .

