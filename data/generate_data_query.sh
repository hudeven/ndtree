#!/bin/sh
python data_generator.py --output data_random --alphabet 4  --dimension 16 --count 1000
python query_generator.py --output query_random --alphabet 4 --dimension 16 --count 200
