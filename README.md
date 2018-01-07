# Tool to reorganize feature vectors by column

This tool can be used to reorganize feature verctors by column. In the process of feature selection it is usual to have the need to reorganize the columns of a large feature vector, a very time consuming process where this application can truly help.

## Development setup

* Ubuntu 16.04 LTS
* CodeBlocks 16.01
* OpenCV 3.2.0

## Aplication features

* 1 - reorganize feature vectors by column.

## Input

Any feature vector in CSV format.(e.g:input.csv).
The pretended columns and order to retain(e.g:input_cols.csv)

## Output

* input_reorg.csv - The reorganized feature vector file.

## Usage

 ./tool-reorg [inputFile] [inputColsFile]

 ### Arguments:

* -h, help message

### Example

Here goes a simple example on how to use this tool with the example file provided with this project(example.csv).

![figure 1](/images/tool-reorg.png)
*figure 1 - Example of column reorganizing.*

