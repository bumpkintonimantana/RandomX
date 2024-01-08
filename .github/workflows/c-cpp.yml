name: Build and Run RandomX Example

on: [push, pull_request]

jobs:
  build-and-run:
    runs-on: ubuntu-22.04

    steps:
      - name: Checkout repository
        uses: actions/checkout@v3

      - name: Install dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y git build-essential cmake gcc g++ make librandomx-dev

      - name: Compile RandomX Example
        run: |
          g++ randomx_example.cpp -o randomx_example -lrandomx -pthread

      - name: Build RandomX
        run: |
          mkdir -p build
          cd build
          cmake ..
          make -j$(nproc)

      - name: Run RandomX Example
        run: |
          cd build
          ../randomx_example
