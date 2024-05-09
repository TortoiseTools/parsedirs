# Parsedirs SW
It can list the files in the system. And it can find files containing the sought sentence fragment among the files in the system.


```
git clone https://github.com/TortoiseTools/parsedirs.git
```

```
cd parsedirs
```
# parsedirsnative
```
gcc -o parsedirsnative parsedirsnative.c -w
```
```
sudo chmod +x parsedirsnative
```
```
sudo ln -s $(pwd)/parsedirsnative /usr/bin/parsedirsnative
```

# parsedirsnativec
```
gcc -o parsedirsnativec parsedirsnativec.c -w
```
```
sudo chmod +x parsedirsnativec
```
```
sudo ln -s $(pwd)/parsedirsnativec /usr/bin/parsedirsnativec
```


# usage
```
parsedirsnativec findfiles [path]
```
```
parsedirsnativec findinfiles [path] [word/sentence]
```

![image](https://github.com/TortoiseTools/parsedirs/assets/141079564/f0ed9d75-b76a-4d07-9dd2-62842d69f150)
![image](https://github.com/TortoiseTools/parsedirs/assets/141079564/8ba267e4-c987-48ab-aeae-e8152b77a7b4)
![image](https://github.com/TortoiseTools/parsedirs/assets/141079564/a963d5f5-5919-44ef-b1d1-ee08ca05271e)
![image](https://github.com/TortoiseTools/parsedirs/assets/141079564/45e9bc13-d500-4938-90c1-44e896d9a2d9)
![image](https://github.com/TortoiseTools/parsedirs/assets/141079564/f4eb376f-08e1-494c-b06f-50640848ce04)


