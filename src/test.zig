const std = @import("std");
const lox = @cImport({
    @cInclude("lox.h");
});
const lox_state_open = lox.lox_state_open;
const lox_stase_close = lox.lox_state_close;
const testing = std.testing;
test "version" {
    const l = lox_state_open();
    defer lox_stase_close(l);
    const ver_raw: [*:0]const u8 = @ptrCast(lox.lox_version_string());
    const sent = ver_raw[0..std.mem.indexOfSentinel(u8, 0, ver_raw) :0];
    try testing.expectEqualStrings("0.0.0-alpha\n", sent);
}
// test "hi" {
//     const l = lox_state_open();
//     defer lox_stase_close(l);
//     try testing.expectEqual(420, lox.say_hi(l));
// }
