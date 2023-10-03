#ifndef MCP2515_H_
#define MCP2515_H_
#endif

void mcp_init();
uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t data);
void mcp_request_send();
uint8_t mcp_read_status();
void mcp_bit_modify();
void mcp_reset();