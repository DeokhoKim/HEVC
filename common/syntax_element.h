#ifndef _SYNTAX_ELEMENT_H_
#define _SYNTAX_ELEMENT_H_

namespace HEVC
{

class InputBitstream;
class OutputBitstream;

namespace SyntaxElement
{

// syntax elemement parsing
unsigned int read_code(InputBitstream* bs, int len);
unsigned int read_uvlc(InputBitstream* bs);
unsigned int read_flag(InputBitstream* bs);
int read_svlc(InputBitstream* bs);

// syntax element writing
void write_code(OutputBitstream* bs, unsigned int code, int len);
void write_uvlc(OutputBitstream* bs, unsigned int code);
void write_flag(OutputBitstream* bs, unsigned int code);
void write_svlc(OutputBitstream* bs, int code);

} // namespace SyntaxElement

} // namespace HEVC

#endif // _SYNTAX_ELEMENT_H_