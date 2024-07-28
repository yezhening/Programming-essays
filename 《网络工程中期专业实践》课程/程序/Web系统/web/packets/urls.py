from django.urls import path

from . import views

app_name='packets'
urlpatterns = [
    path('request_list/', views.request_list, name='request_list'),
    path('request_list/<int:packet_number>/',views.show_host, name='show_host'),
    
    path('response_list/', views.response_list, name='response_list'),
    path('response_list/<int:packet_number>/',views.show_payload, name='show_payload'),

    path('request_sourceip_number/', views.request_sourceip_number, name='request_sourceip_number'),
    path('request_type/', views.request_type, name='request_type'),
    
    path('destinationip_number/', views.destinationip_number, name='destinationip_number'),
    path('response_content_type/', views.response_content_type, name='response_content_type'),
    
    path('specify_host_type/', views.specify_host_type, name='specify_host_type'),
    path('alter_host_type/<str:who_obj_domain_name>/',views.alter_host_type, name='alter_host_type'),
    path('show_host_type/', views.show_host_type, name='show_host_type'),

    path('request_frame_size/', views.request_frame_size, name='request_frame_size'),
    path('response_frame_size/', views.response_frame_size, name='response_frame_size'),

    path('analysis_webpage/', views.analysis_webpage, name='analysis_webpage'),
]
