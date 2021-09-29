1. **Simulate the following topology in Mininet. Set the link bandwidth for (s1,s2) and (s1,s3) as 10Mbps. Use iperf3 to test the TCP throughput between h1 to h2, h3, h4.(30 points)**

```
$ sudo mn --custom ~/mininet/custom/topo-2sw-2host.py --topo mytopo
```

<img src="/Users/luyifan/Library/Application Support/typora-user-images/截屏2021-03-05 下午2.59.03.jpg" alt="截屏2021-03-05 下午2.59.03" style="zoom:50%;" /> 

h1-h4 : 44.2 Gb/s

h1-h2 : 9.57 Gb/s

h1-h3 : 9.57 Gb/s  



2. **Now let us set the packet loss rate of the link (s1,s2) and (s1,s3) as 6%. Use iperf3 to test the TCP throughput between h1 to h2, h3, h4 again.(30 points)**

![截屏2021-03-05 下午3.26.50](/Users/luyifan/Library/Application Support/typora-user-images/截屏2021-03-05 下午3.26.50.jpg)

记得用 sudo mn -c 清除拓扑缓存



3. **Let us add another link between s2 and s3. Try to test the connectivity between all the hosts. What would happen? How would you solve the problem?(40 points)**

<img src="/Users/luyifan/Library/Application Support/typora-user-images/截屏2021-03-05 下午3.39.45.jpg" alt="截屏2021-03-05 下午3.39.45" style="zoom:50%;" />

三个switch互联，就不知道如何转发了。ping也会ping不通。 **除了h1-h4！**

<img src="/Users/luyifan/Library/Application Support/typora-user-images/截屏2021-03-05 下午3.51.32.jpg" alt="截屏2021-03-05 下午3.51.32" style="zoom:50%;" />



**solution：**

<img src="/Users/luyifan/Library/Containers/com.tencent.qq/Data/Library/Caches/Images/6CA8EF46914D755B8C55A52642DF971B.jpg" alt="6CA8EF46914D755B8C55A52642DF971B" style="zoom:50%;" />

记得 来和去都要写流表项！