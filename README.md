# WeatherAndEnergyProduction

## Works on:
- Rpi 4B
- ubuntu server 24.04
- python 3.12
- cmake 3.28.3
- gcc 13.3.0

## Useful:
- [Tutorial yt](https://www.youtube.com/watch?v=pKyJg1_pk1w)
- [Repo](https://github.com/open62541/open62541)
- [Tutorials](https://www.open62541.org/doc/v1.4.10/toc.html)

## Steps:
- install all dependencies
- **Building with CMake on Ubuntu or Debian** chapter in tutorials (up to exclude "# select additional features")
- cd examples/ read header of CMakeLists.txt
- **Building the Examples** chapter
- create and set env variables in examples/.env:

```
API_KEY=
UNITS=metric
SERVER_URL=
```
- install GTest: `sudo apt-get install libgtest-dev`
- and GTest mock: `sudo apt-get install libgmock-dev`

## Compliation:
### Via make:
- server `make server`
- client `make client`
- both server and client `make` 

### Via cmake
- collects main CMakeLists.txt and stores results of building in **build/** dir:
    ```
    make build
    ```

## Strukture:
<pre>
ObjectsFolder (NodeId: 85)
│
├── Weather station (Object, NodeId: 1001)
│   │
│   ├── Weather data (Object, NodeId: 2001)
|   |   |
│   |   ├── City1 (Object, NodeId: 2101)
│   |   ├── City2 (Object, NodeId: 2201)
│   |   ├── City3 (Object, NodeId: 2301)
│   |   ├── City4 (Object, NodeId: 2401)
│   |   ├── City5 (Object, NodeId: 2501)
│   |   ├── City6 (Object, NodeId: 2601)
│   |   ├── City7 (Object, NodeId: 2701)
│   |   ├── City8 (Object, NodeId: 2801)
│   |   ├── City9 (Object, NodeId: 2901)
│   |   ├── City10 (Object, NodeId: 2111)
│   |   ├── City11 (Object, NodeId: 2211)
│   |   ├── City12 (Object, NodeId: 2311)
</pre>