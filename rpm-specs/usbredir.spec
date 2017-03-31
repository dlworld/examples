Name:           usbredir
Version:        0.7
Release:        1%{?dist}
Summary:        USB network redirection protocol libraries
Group:          System Environment/Libraries
License:        LGPLv2+
URL:            http://spice-space.org/page/UsbRedir
Source0:        http://spice-space.org/download/%{name}/%{name}-%{version}.tar.bz2
# Some patches from upstream git (drop at next rebase)
BuildRequires:  libusb1-devel >= 1.0.9

%description
The usbredir libraries allow USB devices to be used on remote and/or virtual
hosts over TCP.  The following libraries are provided:

usbredirparser:
A library containing the parser for the usbredir protocol

usbredirhost:
A library implementing the USB host side of a usbredir connection.
All that an application wishing to implement a USB host needs to do is:
* Provide a libusb device handle for the device
* Provide write and read callbacks for the actual transport of usbredir data
* Monitor for usbredir and libusb read/write events and call their handlers


%package        devel
Summary:        Development files for %{name}
Group:          Development/Libraries
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%package        server
Summary:        Simple USB host TCP server
Group:          System Environment/Daemons
License:        GPLv2+
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    server
A simple USB host TCP server, using libusbredirhost.


%prep
%setup -q


%build
%configure --disable-static
make %{?_smp_mflags} V=1


%install
%make_install
rm $RPM_BUILD_ROOT%{_libdir}/libusbredir*.la


%post -p /sbin/ldconfig
%postun -p /sbin/ldconfig


%files
%doc ChangeLog COPYING.LIB README TODO 
%{_libdir}/libusbredir*.so.*

%files devel
%doc usb-redirection-protocol.txt README.multi-thread
%{_includedir}/usbredir*.h
%{_libdir}/libusbredir*.so
%{_libdir}/pkgconfig/libusbredir*.pc

%files server
%doc COPYING
%{_sbindir}/usbredirserver
%{_mandir}/man1/usbredirserver.1*


%changelog
* Wed Jul 13 2016 Dlw <denglinwen@os-easy.com> - 0.7.1-1
- Mass rebuild 2016-07-13

* Fri Jan 24 2014 Daniel Mach <dmach@redhat.com> - 0.6-7
- Mass rebuild 2014-01-24

* Fri Dec 27 2013 Daniel Mach <dmach@redhat.com> - 0.6-6
- Mass rebuild 2013-12-27

* Tue Sep 10 2013 Hans de Goede <hdegoede@redhat.com> - 0.6-5
- Use the new libusb autodetach kernel driver functionality
- Fix a usbredirparser bug which causes tcp/ip redir to not work (rhbz#1005015)

* Sun Aug 04 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.6-4
- Rebuilt for https://fedoraproject.org/wiki/Fedora_20_Mass_Rebuild

* Mon May 13 2013 Hans de Goede <hdegoede@redhat.com> - 0.6-3
- Fix usbredirserver not listening for ipv6 connections (rhbz#957470)
- Fix a few (harmless) coverity warnings

* Fri Feb 15 2013 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.6-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_19_Mass_Rebuild

* Thu Dec 13 2012 Hans de Goede <hdegoede@redhat.com> - 0.6-1
- Update to upstream 0.6 release

* Tue Sep 25 2012 Hans de Goede <hdegoede@redhat.com> - 0.5.2-1
- Update to upstream 0.5.2 release

* Wed Sep 19 2012 Hans de Goede <hdegoede@redhat.com> - 0.5.1-1
- Update to upstream 0.5.1 release

* Fri Sep  7 2012 Hans de Goede <hdegoede@redhat.com> - 0.5-1
- Update to upstream 0.5 release

* Mon Jul 30 2012 Hans de Goede <hdegoede@redhat.com> - 0.4.3-3
- Add 2 fixes from upstream fixing issues with some bulk devices (rhbz#842358)

* Sun Jul 22 2012 Fedora Release Engineering <rel-eng@lists.fedoraproject.org> - 0.4.3-2
- Rebuilt for https://fedoraproject.org/wiki/Fedora_18_Mass_Rebuild

* Mon Apr  2 2012 Hans de Goede <hdegoede@redhat.com> - 0.4.3-1
- Update to upstream 0.4.3 release

* Tue Mar  6 2012 Hans de Goede <hdegoede@redhat.com> - 0.4.2-1
- Update to upstream 0.4.2 release

* Sat Feb 25 2012 Hans de Goede <hdegoede@redhat.com> - 0.4.1-1
- Update to upstream 0.4.1 release

* Thu Feb 23 2012 Hans de Goede <hdegoede@redhat.com> - 0.4-1
- Update to upstream 0.4 release

* Thu Jan 12 2012 Hans de Goede <hdegoede@redhat.com> - 0.3.3-1
- Update to upstream 0.3.3 release

* Tue Jan  3 2012 Hans de Goede <hdegoede@redhat.com> 0.3.2-1
- Update to upstream 0.3.2 release

* Wed Aug 24 2011 Hans de Goede <hdegoede@redhat.com> 0.3.1-1
- Update to upstream 0.3.1 release

* Thu Jul 14 2011 Hans de Goede <hdegoede@redhat.com> 0.3-1
- Initial Fedora package
