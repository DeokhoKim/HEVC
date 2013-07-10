#ifndef _SYNTAX_ELEMENT_H_
#define _SYNTAX_ELEMENT_H_

namespace HEVC
{

class InputBitstream;
class OutputBitstream;

namespace SyntaxElement
{

// syntax elemement parsing
unsigned int read_code(InputBitstream* bistream, int len);
unsigned int read_uvlc(InputBitstream* bitstream);
unsigned int read_flag(InputBitstream* bitstream);
int read_svlc(InputBitstream* bitstream);

// syntax element writing
void write_code(OutputBitstream* bitstream, unsigned int code, int len);
void write_uvlc(OutputBitstream* bitstream, unsigned int code);
void write_flag(OutputBitstream* bitstream, unsigned int code);
void write_svlc(OutputBitstream* bitstream, int code);

} // namespace SyntaxElement

} // namespace HEVC

#endif // _SYNTAX_ELEMENT_H_