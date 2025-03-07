# SPDX-FileCopyrightText: 2011-2022 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

import sys
import os


# may split this out into a new file
def replace_bpy_app_version():
    """ So MD5's are predictable from output which uses blenders versions.
    """

    import bpy

    app = bpy.app
    app_fake = type(bpy)("bpy.app")

    for attr in dir(app):
        if not attr.startswith("_"):
            setattr(app_fake, attr, getattr(app, attr))

    app_fake.version = 0, 0, 0
    app_fake.version_string = "0.00 (sub 0)"
    bpy.app = app_fake


def clear_startup_blend():
    import bpy

    for col in bpy.data.collections:
        for obj in col.objects:
            col.objects.unlink(obj)


def blend_to_md5():
    import bpy
    scene = bpy.context.scene
    ROUND = 4

    def matrix2str(matrix):
        return "".join([str(round(axis, ROUND)) for vector in matrix for axis in vector]).encode('ASCII')

    def coords2str(seq, attr):
        return "".join([str(round(axis, ROUND)) for vertex in seq for axis in getattr(vertex, attr)]).encode('ASCII')

    import hashlib

    md5 = hashlib.new("md5")
    md5_update = md5.update

    for obj in scene.objects:
        md5_update(matrix2str(obj.matrix_world))
        data = obj.data

        if type(data) == bpy.types.Mesh:
            md5_update(coords2str(data.vertices, "co"))
        elif type(data) == bpy.types.Curve:
            for spline in data.splines:
                md5_update(coords2str(spline.bezier_points, "co"))
                md5_update(coords2str(spline.points, "co"))

    return md5.hexdigest()


def main():
    argv = sys.argv
    print("  args:", " ".join(argv))
    argv = argv[argv.index("--") + 1:]

    def arg_extract(arg, optional=True, array=False):
        arg += "="
        if array:
            value = []
        else:
            value = None

        i = 0
        while i < len(argv):
            if argv[i].startswith(arg):
                item = argv[i][len(arg):]
                del argv[i]
                i -= 1

                if array:
                    value.append(item)
                else:
                    value = item
                    break

            i += 1

        if (not value) and (not optional):
            print("  '%s' not set" % arg)
            sys.exit(1)

        return value

    run = arg_extract("--run", optional=False)
    md5 = arg_extract("--md5", optional=False)
    md5_method = arg_extract("--md5_method", optional=False)  # 'SCENE' / 'FILE'

    # only when md5_method is 'FILE'
    md5_source = arg_extract("--md5_source", optional=True, array=True)

    # save blend file, for testing
    write_blend = arg_extract("--write-blend", optional=True)

    # ensure files are written anew
    for f in md5_source:
        if os.path.exists(f):
            os.remove(f)

    import bpy

    replace_bpy_app_version()
    if not bpy.data.filepath:
        clear_startup_blend()

    print("  Running: '%s'" % run)
    print("  MD5: '%s'!" % md5)

    result = eval(run)

    if write_blend is not None:
        print("  Writing Blend: %s" % write_blend)
        bpy.ops.wm.save_mainfile('EXEC_DEFAULT', filepath=write_blend)

    print("  Result: '%s'" % str(result))
    if not result:
        print("  Running: %s -> False" % run)
        sys.exit(1)

    if md5_method == 'SCENE':
        md5_new = blend_to_md5()
    elif md5_method == 'FILE':
        if not md5_source:
            print("  Missing --md5_source argument")
            sys.exit(1)

        for f in md5_source:
            if not os.path.exists(f):
                print("  Missing --md5_source=%r argument does not point to a file")
                sys.exit(1)

        import hashlib

        md5_instance = hashlib.new("md5")
        md5_update = md5_instance.update

        for f in md5_source:
            with open(f, "rb") as fh:
                md5_update(fh.read())

        md5_new = md5_instance.hexdigest()

    else:
        print("  Invalid --md5_method=%s argument is not a valid source")
        sys.exit(1)

    if md5 != md5_new:
        print("  Running: %s\n    MD5 Received: %s\n    MD5 Expected: %s" % (run, md5_new, md5))
        sys.exit(1)

    print("  Success: %s" % run)


if __name__ == "__main__":
    main()
