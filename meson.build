project ('gtk3-classic-module', 'c',
	version : '0.89.1',
	default_options : [
		'buildtype=release',
		'optimization=2',
		'prefix=/usr',
		'strip=true',
		'warning_level=3'
	],
	license: 'LGPLv2')


gtk_version_required = '>= 3.24.0'
gtk_dep = dependency ('gtk+-3.0',
	version: gtk_version_required)
gtk_unix_print_dep = dependency ('gtk+-unix-print-3.0',
	version: gtk_version_required)
# For glib-compile-schemas
gnome = import ('gnome')


gtk_libdir = gtk_dep.get_variable (
	pkgconfig: 'libdir',
	internal: 'libdir',
	default_value: get_option ('libdir'))

gtk_sysconfdir = gtk_dep.get_variable (
	pkgconfig: 'sysconfdir',
	internal: 'sysconfdir',
	default_value: get_option ('sysconfdir'))

gtk_datadir = gtk_dep.get_variable (
	pkgconfig: 'datadir',
	internal: 'datadir',
	default_value: get_option ('datadir'))


modules_subdir = 'gtk-3.0/modules'
modules_install_dir = join_paths (gtk_libdir, modules_subdir)

settings_subdir = 'gtk-3.0'
settings_install_dir = join_paths (gtk_sysconfdir, settings_subdir)

schemas_subdir = 'glib-2.0/schemas'
schemas_install_dir = join_paths (gtk_datadir, schemas_subdir)


include_dir = include_directories ('include')


shared_module ('gtk3-classic-module',
	sources: [
		'main.c',
		'patches/button-menu-icons.c',
		'patches/dialogs.c',
		'patches/icon-sizes.c',
		'patches/no-emojis.c',
		'patches/persistent-mnemonics.c',
		'patches/smaller-widgets.c'
	],
	include_directories: [ include_dir ],
	dependencies: [ gtk_dep, gtk_unix_print_dep ],
	install_dir: modules_install_dir,
	install: true
)


install_data ('data/settings.ini',
	install_dir: settings_install_dir)

schema_file = 'data/00_gtk3-classic-module.gschema.override'
install_data (schema_file,
	install_dir: schemas_install_dir)
gnome.post_install (glib_compile_schemas: true)
