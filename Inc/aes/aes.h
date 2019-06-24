

#ifndef AES_H
#define AES_H

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#if 1
# define AES_ENC_PREKEYED  /* AES encryption with a precomputed key schedule  */
#endif
#if 1
# define AES_DEC_PREKEYED  /* AES decryption with a precomputed key schedule  */
#endif


#define N_ROW 4
#define N_COL 4
#define N_BLOCK (N_ROW * N_COL)
#define N_MAX_ROUNDS 14

typedef unsigned char uint_8t;

typedef uint_8t return_type;

/*  Warning: The key length for 256 bit keys overflows a byte
(see comment below)
*/

typedef uint_8t length_type;

typedef struct
{
	uint_8t ksch[(N_MAX_ROUNDS + 1) * N_BLOCK];
	uint_8t rnd;
} aes_context;

/*  The following calls are for a precomputed key schedule

NOTE: If the length_type used for the key length is an
unsigned 8-bit character, a key length of 256 bits must
be entered as a length in bytes (valid inputs are hence
128, 192, 16, 24 and 32).
*/

#if defined( AES_ENC_PREKEYED ) || defined( AES_DEC_PREKEYED )

return_type aes_set_key( const unsigned char key[],
								length_type keylen,
								aes_context ctx[1] );
#endif

#if defined( AES_ENC_PREKEYED )

return_type aesencrypt( const unsigned char in[N_BLOCK],
							unsigned char out[N_BLOCK],
							const aes_context ctx[1] );

return_type aes_cbc_encrypt( const unsigned char *in,
								unsigned char *out,
								int n_block,
								unsigned char iv[N_BLOCK],
								const aes_context ctx[1] );
#endif

#if defined( AES_DEC_PREKEYED )

return_type aesdecrypt( const unsigned char in[N_BLOCK],
						unsigned char out[N_BLOCK],
						const aes_context ctx[1] );

return_type aes_cbc_decrypt( const unsigned char *in,
								unsigned char *out,
								int n_block,
								unsigned char iv[N_BLOCK],
								const aes_context ctx[1] );
#endif


#endif
