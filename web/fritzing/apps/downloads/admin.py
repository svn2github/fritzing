from django.contrib import admin
from django.utils.translation import ugettext_lazy as _

from downloads.models import Platform, Release, Download

class PlatformAdmin(admin.ModelAdmin):
    list_display = ('name', 'releases')
    prepopulated_fields = {'slug': ('name',)}

    def releases(self, obj):
        return ", ".join(['<a href="../release/%s/">%s</a>' % (download.release.pk,
            download.release.version) for download in obj.download_set.all()])
    releases.short_description = 'Releases'
    releases.allow_tags = True

class DownloadInline(admin.TabularInline):
    model = Download
    extra = 4

class ReleaseAdmin(admin.ModelAdmin):
    fieldsets = (
        (None, {
            'fields':
                ('active', 'version', 'description', 'type', 'release_date'),
        }),
        (_('Details'), {
            'fields': ('changelog', 'known_issues'),
        }),
    )

    list_display = ('version', 'release_date', 'type', 'active', 'downloads', 'updates')
    list_filter = ('release_date', 'version', 'type', 'active')
    search_fields = ['changelog']
    inlines = [DownloadInline]

admin.site.register(Platform, PlatformAdmin)
admin.site.register(Release, ReleaseAdmin)
