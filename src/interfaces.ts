/**
 * These are the options available when using the compression function.
 * The options provided are exactly the arguments that the libzpaq::compress() function takes in.
 * All of these are optional and have defaults provided by the bindings.
 * For more information on these, see libzpaq.h.
 */
export interface CompressionOptions {
    method?: string;
    fileName?: string;
    comment?: string;
    sha1?: boolean;
}
