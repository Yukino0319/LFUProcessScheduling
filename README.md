# LFUProcessScheduling
### update at 2017/06/21
  转码至UTF-8<br />
  增加了一个周期计数器，当未发生缺页中断次数超过周期时，page_count清零
### LFU（Least Frequently Used）最近最少使用算法。它是基于“如果一个数据在最近一段时间内使用次数很少，那么在将来一段时间内被使用的可能性也很小”的思路。
### LFU: 从0开始, 命中的页面每次计数器加1, 替换时将计数最小的踢出。
